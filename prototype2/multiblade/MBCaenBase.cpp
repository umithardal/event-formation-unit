/** Copyright (C) 2017-2018 European Spallation Source */
//===----------------------------------------------------------------------===//
///
/// \file
/// Implementation of the detector pipeline plugin for MUlti-Blade
/// detectors.
/// Contributor: Carsten Søgaard, Niels Bohr Institute, University of Copenhagen
//===----------------------------------------------------------------------===//

#include "MBCaenBase.h"

#include <cinttypes>
#include <common/EFUArgs.h>
#include <common/EV42Serializer.h>
#include <common/Producer.h>
#include <common/HistSerializer.h>
#include <common/RingBuffer.h>
#include <common/Trace.h>
#include <common/TimeString.h>
#include <unistd.h>

#include <libs/include/SPSCFifo.h>
#include <libs/include/Socket.h>
#include <libs/include/TSCTimer.h>
#include <libs/include/Timer.h>

#include <caen/DataParser.h>

#include <clustering/EventBuilder.h>

#include <logical_geometry/ESSGeometry.h>
#include <caen/MBGeometry.h>

//#undef TRC_LEVEL
//#define TRC_LEVEL TRC_L_DEB

namespace Multiblade {

using namespace memory_sequential_consistent; // Lock free fifo

const char *classname = "Multiblade detector with CAEN readout";

const int TSC_MHZ = 2900; // MJC's workstation - not reliable


CAENBase::CAENBase(BaseSettings const &settings, struct CAENSettings &LocalMBCAENSettings)
    : Detector("MBCAEN", settings), MBCAENSettings(LocalMBCAENSettings) {
  Stats.setPrefix("efu.mbcaen");

  XTRACE(INIT, ALW, "Adding stats");
  // clang-format off
  Stats.create("input.rx_packets", mystats.rx_packets);
  Stats.create("input.rx_bytes", mystats.rx_bytes);
  Stats.create("input.fifo1_push_errors", mystats.fifo1_push_errors);
  Stats.create("processing.rx_readouts", mystats.rx_readouts);
  Stats.create("processing.rx_error_bytes", mystats.rx_error_bytes);
  Stats.create("processing.rx_seq_errors", mystats.rx_seq_errors);
  Stats.create("processing.rx_idle1", mystats.rx_idle1);
  Stats.create("processing.tx_bytes", mystats.tx_bytes);
  Stats.create("processing.rx_events", mystats.rx_events);
  Stats.create("processing.rx_geometry_errors", mystats.geometry_errors);
  Stats.create("processing.fifo_seq_errors", mystats.fifo_seq_errors);
  /// \todo below stats are common to all detectors and could/should be moved
  Stats.create("kafka_produce_fails", mystats.kafka_produce_fails);
  Stats.create("kafka_ev_errors", mystats.kafka_ev_errors);
  Stats.create("kafka_ev_others", mystats.kafka_ev_others);
  Stats.create("kafka_dr_errors", mystats.kafka_dr_errors);
  Stats.create("kafka_dr_others", mystats.kafka_dr_noerrors);
  // clang-format on

  std::function<void()> inputFunc = [this]() { CAENBase::input_thread(); };
  Detector::AddThreadFunction(inputFunc, "input");

  std::function<void()> processingFunc = [this]() {
    CAENBase::processing_thread();
  };
  Detector::AddThreadFunction(processingFunc, "processing");

  XTRACE(INIT, ALW, "Creating %d Multiblade Rx ringbuffers of size %d",
         eth_buffer_max_entries, eth_buffer_size);
  /// \todo the number 11 is a workaround
  eth_ringbuf = new RingBuffer<eth_buffer_size>(eth_buffer_max_entries + 11);
  assert(eth_ringbuf != 0);

  mb_opts = Config(MBCAENSettings.ConfigFile);
  assert(mb_opts.getDetector() != nullptr);
}

void CAENBase::input_thread() {
  /** Connection setup */
  Socket::Endpoint local(EFUSettings.DetectorAddress.c_str(),
                         EFUSettings.DetectorPort);
  UDPReceiver receiver(local);
  // receiver.buflen(opts->buflen);
  receiver.setBufferSizes(0, EFUSettings.DetectorRxBufferSize);
  receiver.printBufferSizes();
  receiver.setRecvTimeout(0, 100000); /// secs, usecs 1/10s

  int rdsize;
  for (;;) {
    unsigned int eth_index = eth_ringbuf->getDataIndex();

    /** this is the processing step */
    eth_ringbuf->setDataLength(eth_index, 0);
    if ((rdsize = receiver.receive(eth_ringbuf->getDataBuffer(eth_index),
                                   eth_ringbuf->getMaxBufSize())) > 0) {
      eth_ringbuf->setDataLength(eth_index, rdsize);
      XTRACE(INPUT, DEB, "Received an udp packet of length %d bytes",
             rdsize);
      mystats.rx_packets++;
      mystats.rx_bytes += rdsize;

      if (input2proc_fifo.push(eth_index) == false) {
        mystats.fifo1_push_errors++;
      } else {
        eth_ringbuf->getNextBuffer();
      }
    }

    // Checking for exit
    if (not runThreads) {
      XTRACE(INPUT, ALW, "Stopping input thread.");
      return;
    }
  }
}

void CAENBase::processing_thread() {
  const uint32_t ncass = 6;
  uint8_t nwires = 32;
  uint8_t nstrips = 32;
  std::string topic{""};
  std::string monitor{""};

  MBGeometry mbgeom(ncass, nwires, nstrips);
  ESSGeometry *essgeom;
  if (mb_opts.getInstrument() == Config::InstrumentGeometry::Estia) {
    XTRACE(PROCESS, ALW, "Setting instrument configuration to Estia");
    mbgeom.setConfigurationEstia();
    essgeom = new ESSGeometry(nstrips, ncass * nwires, 1, 1);
    topic = "ESTIA_detector";
    monitor = "ESTIA_monitor";
  } else {
    mbgeom.setConfigurationFreia();
    XTRACE(PROCESS, ALW, "Setting instrument configuration to Freia");
    essgeom = new ESSGeometry(ncass * nwires, nstrips, 1, 1);
    topic = "FREIA_detector";
    monitor = "FREIA_monitor";
  }

  if (mb_opts.getDetectorType() == Config::DetectorType::MB18) {
    XTRACE(PROCESS, ALW, "Setting detector to MB18");
    mbgeom.setDetectorMB18();
  } else {
    XTRACE(PROCESS, ALW, "Setting detector to MB16");
    mbgeom.setDetectorMB16();
  }

  std::shared_ptr<ReadoutFile> dumpfile;
  if (!MBCAENSettings.FilePrefix.empty()) {
    dumpfile = ReadoutFile::create(
        MBCAENSettings.FilePrefix + "-" + timeString());
  }

  EV42Serializer flatbuffer(kafka_buffer_size, "multiblade");
  Producer eventprod(EFUSettings.KafkaBroker, topic);
  flatbuffer.setProducerCallback(
      std::bind(&Producer::produce2<uint8_t>, &eventprod, std::placeholders::_1));

  Hists histograms(std::max(ncass * nwires, ncass * nstrips), 65535);
  Producer monitorprod(EFUSettings.KafkaBroker, monitor);
  HistSerializer histfb(histograms.needed_buffer_size());
  histfb.set_callback(
      std::bind(&Producer::produce2<uint8_t>, &monitorprod, std::placeholders::_1));

  EventBuilder builder[ncass];
  for (uint32_t i = 0; i < ncass; i++) {
    builder[i].setNumberOfWireChannels(nwires);
    builder[i].setNumberOfStripChannels(nstrips);
  }

  DataParser parser;
  auto digitisers = mb_opts.getDigitisers();
  DigitizerMapping mb1618(digitisers);


  unsigned int data_index;
  TSCTimer produce_timer;
  Timer  h5flushtimer;
  while (1) {
    if ((input2proc_fifo.pop(data_index)) == false) {
      // There is NO data in the FIFO - do stop checks and sleep a little
      mystats.rx_idle1++;

      // if filedumping and requesting time splitting, check for rotation.
      if (MBCAENSettings.H5SplitTime != 0 and (dumpfile)) {
        if (h5flushtimer.timeus() >= MBCAENSettings.H5SplitTime * 1000000) {

          /// \todo user should not need to call flush() - implicit in rotate() ?
          dumpfile->flush();
          dumpfile->rotate();
          h5flushtimer.now();
        }
      }

      if (produce_timer.timetsc() >=
          EFUSettings.UpdateIntervalSec * 1000000 * TSC_MHZ) {

        mystats.tx_bytes += flatbuffer.produce();

        if (!histograms.isEmpty()) {
          XTRACE(PROCESS, INF, "Sending histogram for %zu readouts",
              histograms.hit_count());
          histfb.produce(histograms);
          histograms.clear();
        }

        /// Kafka stats update - common to all detectors
        /// don't increment as producer keeps absolute count
        mystats.kafka_produce_fails = eventprod.stats.produce_fails;
        mystats.kafka_ev_errors = eventprod.stats.ev_errors;
        mystats.kafka_ev_others = eventprod.stats.ev_others;
        mystats.kafka_dr_errors = eventprod.stats.dr_errors;
        mystats.kafka_dr_noerrors = eventprod.stats.dr_noerrors;

        if (not runThreads) {
          XTRACE(INPUT, ALW, "Stopping processing thread.");
          return;
        }

        produce_timer.now();
      }
      usleep(10);

    } else { // There is data in the FIFO - do processing
      auto datalen = eth_ringbuf->getDataLength(data_index);
      if (datalen == 0) {
        mystats.fifo_seq_errors++;
      } else {
        /// \todo use the Buffer<T> class here and in parser
        auto dataptr = eth_ringbuf->getDataBuffer(data_index);
        if (parser.parse(dataptr, datalen) < 0) {
          mystats.rx_error_bytes += parser.Stats.error_bytes;
          continue;
        }
        mystats.rx_seq_errors += parser.Stats.seq_errors;

        XTRACE(DATA, DEB, "Received %d readouts from digitizer %d",
               parser.MBHeader->numElements, parser.MBHeader->digitizerID);

        mystats.rx_readouts += parser.MBHeader->numElements;

        if (dumpfile) {
          dumpfile->push(parser.readouts);
          XTRACE(DATA, DEB, "Pushed %d readouts to dumpfile",
                 parser.readouts.size());
        }

        /// \todo why can't I use mb_opts.detector->cassette()
        auto cassette = mb1618.cassette(parser.MBHeader->digitizerID);
        if (cassette < 0) {
          XTRACE(DATA, WAR, "Invalid digitizerId: %d",
                 parser.MBHeader->digitizerID);
          continue;
        }

        for (const auto &dp : parser.readouts) {

          assert(dp.channel < nwires + nstrips);

          uint16_t coord; // \todo invalidate ?
          int plane = mbgeom.getPlane(dp.channel);

          if ( plane == 0) {
            coord = mbgeom.getx(0, dp.channel);
            histograms.binstrips(mbgeom.getx(cassette, dp.channel), dp.adc, 0, 0);
          }

          if (plane == 1) {
            coord = mbgeom.gety(0, dp.channel);
            histograms.binstrips(0, 0, mbgeom.gety(cassette, dp.channel), dp.adc);
          }
             //builder[cassette].addDataPoint(plane, coord, dp.adc, dp.local_time);
        }
        // calculate local x and y
        // localx = event.x();
        // localy = event.y();
        // time = event.time();

        //pixel = mbgeom.getPixel(cassette, localx, localy);
        static uint32_t time = 0;
        static uint32_t pixel_id = 1;

        if (pixel_id == 0) {
          mystats.geometry_errors++;
        } else {
          uint32_t pixel;
          if (time % 10000 == 0) {
            pixel = 6144;
          } else {
            pixel = pixel_id % 700;
          }
          mystats.tx_bytes += flatbuffer.addEvent(time, pixel);
          mystats.rx_events++;
        }
        time++;
        pixel_id++;
        /// \todo we need to also flush leftover clusters in case of loop termination
      }
    }
  }
}

}
