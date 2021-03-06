/** Copyright (C) 2019 European Spallation Source ERIC */

/** @file
 *
 *  \brief Unit tests.
 */

#include <string>

#include <loki/LokiBase.h>
#include <../src/adc_readout/test/TestUDPServer.h>
#include <test/SaveBuffer.h>
#include <test/TestBase.h>
#include <readout/ReadoutParser.h>

 std::string lokijson = R"(
{
  "Detector": "LoKI4x8",

  "PanelConfig" : [
    { "Ring" : 0, "Vertical" :  true,  "TubesZ" : 4, "TubesN" : 8, "Offset" :      0 },
    { "Ring" : 1, "Vertical" :  false, "TubesZ" : 4, "TubesN" : 8, "Offset" : 114688 }
  ]
}
)";

class LokiBaseStandIn : public Loki::LokiBase {
public:
  LokiBaseStandIn(BaseSettings Settings, struct Loki::LokiSettings ReadoutSettings)
      : Loki::LokiBase(Settings, ReadoutSettings){};
  ~LokiBaseStandIn() = default;
  using Detector::Threads;
  using Loki::LokiBase::Counters;
};

class LokiBaseTest : public ::testing::Test {
public:
  void SetUp() override {
    Settings.RxSocketBufferSize = 100000;
    Settings.NoHwCheck = true;
    LocalSettings.ConfigFile = "deleteme_loki.json";
  }
  void TearDown() override {}

  std::chrono::duration<std::int64_t, std::milli> SleepTime{400};
  BaseSettings Settings;
  Loki::LokiSettings LocalSettings;
};

TEST_F(LokiBaseTest, Constructor) {
  LokiBaseStandIn Readout(Settings, LocalSettings);
  EXPECT_EQ(Readout.Counters.RxPackets, 0);
}


std::vector<uint8_t> TestPacket{0x00, 0x01, 0x02};

std::vector<uint8_t> TestPacket2{
    EXTRA_PADDING
    0x45, 0x53, 0x53, 0x00, //  'E' 'S' 'S' 0x00
    0x30, 0x00, 0x9c + PAD_SIZE, 0x00, // 0x009c = 156
    0x11, 0x00, 0x00, 0x00, // Pulse time High (17s)
    0x00, 0x01, 0x00, 0x00, // Pulse time Low (256 clocks)
    0x10, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, // Seq number 1

    0x05, 0x00, 0x40, 0x00, // Data Header, ring 0, fen 0

    0x11, 0x00, 0x00, 0x00, // Readout 1, time 17s,
    0x01, 0x01, 0x00, 0x00, // (257 clocks)
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x02, 0x01,
    0x03, 0x01, 0x04, 0x01,

    0x11, 0x00, 0x00, 0x00, // Readout 2, time 17s
    0x02, 0x01, 0x00, 0x00, // (258 clocks)
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x02, 0x02,
    0x03, 0x02, 0x04, 0x02,

    0x11, 0x00, 0x00, 0x00, // Readout 3, time 17s
    0x03, 0x01, 0x00, 0x00, // (259 clocks)
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x03, 0x02, 0x03,
    0x03, 0x03, 0x04, 0x03,

    0x01, 0x01, 0x40, 0x00, // Data Header 2, ring 1, fen 1

    0x11, 0x00, 0x00, 0x00, // Readout 4, time 1
    0x01, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x02, 0x01,
    0x03, 0x01, 0x04, 0x01,

    0x11, 0x00, 0x00, 0x00, // Readout 5
    0x02, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x02, 0x02,
    0x03, 0x02, 0x04, 0x02,

    0x11, 0x00, 0x00, 0x00, // Readout 6
    0x03, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, // all amplitudes are 0 -> GeometryError++
};

TEST_F(LokiBaseTest, DataReceive) {
  Settings.DetectorPort = 9000;
  LokiBaseStandIn Readout(Settings, LocalSettings);
  Readout.startThreads();

  std::this_thread::sleep_for(SleepTime);
  TestUDPServer Server(43126, Settings.DetectorPort, (unsigned char *)&TestPacket[0], TestPacket.size());
  Server.startPacketTransmission(1, 100);
  std::this_thread::sleep_for(SleepTime);
  Readout.stopThreads();
  EXPECT_EQ(Readout.Counters.RxPackets, 1);
  EXPECT_EQ(Readout.Counters.RxBytes, TestPacket.size());
  EXPECT_EQ(Readout.Counters.Readouts, 0);
}

TEST_F(LokiBaseTest, DataReceiveGood) {
  Settings.DetectorPort = 9001;
  Settings.UpdateIntervalSec = 0;
  LocalSettings.FilePrefix = "deleteme_";
  LokiBaseStandIn Readout(Settings, LocalSettings);
  Readout.startThreads();

  std::this_thread::sleep_for(SleepTime);
  TestUDPServer Server(43127, Settings.DetectorPort, (unsigned char *)&TestPacket2[0], TestPacket2.size());
  Server.startPacketTransmission(1, 100);
  std::this_thread::sleep_for(SleepTime);
  Readout.stopThreads();
  EXPECT_EQ(Readout.Counters.RxPackets, 1);
  EXPECT_EQ(Readout.Counters.RxBytes, TestPacket2.size());
  EXPECT_EQ(Readout.Counters.Readouts, 6);
  EXPECT_EQ(Readout.Counters.Headers, 2);
  EXPECT_EQ(Readout.Counters.GeometryErrors, 1);
  EXPECT_EQ(Readout.Counters.MappingErrors, 1);
  EXPECT_EQ(Readout.Counters.kafka_ev_errors, 2);
}

int main(int argc, char **argv) {
  std::string filename{"deleteme_loki.json"};
  saveBuffer(filename, (void *)lokijson.c_str(), lokijson.size());

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
