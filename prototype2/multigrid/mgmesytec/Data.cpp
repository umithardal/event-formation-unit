/** Copyright (C) 2017 European Spallation Source ERIC */

#include <arpa/inet.h>
#include <cassert>
#include <common/Trace.h>
#include <cstring>
#include <gdgem/nmx/Hists.h>
#include <multigrid/mgmesytec/Data.h>

// #undef TRC_LEVEL
// #define TRC_LEVEL TRC_L_DEB

void MesytecData::mesytec_parse_n_words(uint32_t *buffer, int nWords, NMXHists &hists) {
  uint32_t *datap = buffer;
  int wordsleft = nWords;

  int time = -1;
  int module = -1;
  int bus = -1;
  int addr = -1;
  int adc = -1;
  int dataWords = -1;

  // Sneak peek on time although it is actually last in packet
  uint32_t *tptr = (buffer + nWords - 1);
  if ((*tptr & 0xc0000000) == 0xc0000000) {
    time = *tptr & 0x3fffffff;
  }

  while (wordsleft > 0) {
    auto datatype = *datap & 0xff000000;

    switch (datatype) {
    case mesytecHeader:
      dataWords = *datap & 0x000003ff;
      assert(nWords > dataWords);
      module = (*datap & 0x00ff0000) >> 16;
      DTRACE(INF, "Data len %d (words), module %d\n", dataWords, module);
      break;

    case mesytecData:
      bus = (*datap & 0x0f000000) >> 24;
      addr = (*datap & 0x00fff000) >> 12; /**< channel */
      adc = (*datap & 0x00000fff);
      readouts++;

      // if ( (mgseq.isWire(addr) && adc >= wireThreshold) ||
      //      (mgseq.isGrid(addr) && adc >= gridThreshold)    )  {
      if (adc >= adcThreshold) { // @todo add other logic
        DTRACE(DEB, "accepting %d,%d,%d,%d\n", time, bus, addr, adc);
        hists.binstrips(addr, adc, 0, 0); // @todo @fixme only one strip at a time

        #ifdef DUMPTOFILE
              mgdata.tofile("%d, %d, %d, %d\n", time, bus, addr, adc);
        #endif
      } else {
        DTRACE(DEB, "discarding %d,%d,%d,%d\n", time, bus, addr, adc);
        discards++;
      }
      break;

    case mesytecTimeOffset:
      bus = (*datap & 0x0f000000) >> 24;
      DTRACE(INF, "Timeoffset (bus %d) %d\n", bus, (*datap & 0x0000ffff));
      break;

    default:
      if ((*datap & mesytecTimeStamp) == mesytecTimeStamp) {
        DTRACE(INF, "Timestamp: %d\n", *datap & 0x3fffffff);
        break;
      }

      if (*datap == 0x00000000) {
        // DTRACE(INF, "End of Data\n");
        break;
      }

      DTRACE(WAR, "Unknown: 0x%08x\n", *datap);
      break;
    }
    wordsleft--;
    datap++;
  }

  if (time == -1 || module == -1) {
    XTRACE(PROCESS, WAR, "time or module not set\n");
    readouts = 0;
  }
}

int MesytecData::parse(const char *buffer, int size, NMXHists &hists) {
  int bytesleft = size;
  readouts = 0;
  discards = 0;

  if (buffer[0] != 0x60) {
    return -error::EUNSUPP;
  }

  if (size < 19) {
    return -error::ESIZE;
  }

  uint32_t *datap = (uint32_t *)(buffer + 3);
  bytesleft -= 3;

  while (bytesleft > 16) {
    if ((*datap & 0x000000ff) != 0x58) {
      return -error::EUNSUPP;
    }

    auto len = ntohs((*datap & 0x00ffff00) >> 8);
    DTRACE(INF, "sis3153 datawords %d\n", len);
    datap++;
    bytesleft -= 4;

    if ((*datap & 0xff000000) != sisBeginReadout) {
      return -error::EHEADER;
    }
    datap++;
    bytesleft -= 4;
    mesytec_parse_n_words(datap, len - 3, hists);

    datap += (len - 3);
    bytesleft -= (len - 3) * 4;

    if (*datap != 0x87654321) {
      XTRACE(PROCESS, WAR, "Protocol mismatch, expected 0x87654321\n");
      return -error::EHEADER;
    }
    datap++;
    bytesleft -= 4;

    if ((*datap & 0xff000000) != sisEndReadout) {
      return -error::EHEADER;
    }
    datap++;
    bytesleft -= 4;
  }
  // printf("bytesleft %d\n", bytesleft);
  return error::OK;
}
