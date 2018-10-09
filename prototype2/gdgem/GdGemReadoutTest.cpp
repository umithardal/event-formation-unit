/** Copyright (C) 2018 European Spallation Source ERIC */

/** @file
 *
 *  \brief Unit tests.
 */

#include "GdGemBase.h"
#include <../prototype2/adc_readout/test/TestUDPServer.h>
#include <gtest/gtest.h>
//#include <random>
//#include <trompeloeil.hpp>

class GdGemBaseStandIn : public GdGemBase {
public:
  GdGemBaseStandIn(BaseSettings Settings, struct NMXSettings ReadoutSettings)
      : GdGemBase(Settings, ReadoutSettings){};
  ~GdGemBaseStandIn() = default;
  using Detector::Threads;
  using GdGemBase::mystats;
};

class GdGemBaseTest : public ::testing::Test {
public:
  virtual void SetUp() {
    LocalSettings.ConfigFile = "../prototype2/gdgem/configs/vmm3.json";
    Settings.DetectorRxBufferSize = 100000;
    Settings.MinimumMTU = 1500;
  }
  virtual void TearDown() {}

  BaseSettings Settings;
  NMXSettings LocalSettings;
};

TEST_F(GdGemBaseTest, Constructor) {
  GdGemBaseStandIn Readout(Settings, LocalSettings);
  Readout.startThreads();
  TestUDPServer Server(43126, Settings.DetectorPort, 1470);
  Server.startPacketTransmission(1, 100);
  std::chrono::duration<std::int64_t, std::milli> SleepTime(200);
  std::this_thread::sleep_for(SleepTime);
  Readout.stopThreads();
  EXPECT_EQ(Readout.mystats.rx_packets, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
