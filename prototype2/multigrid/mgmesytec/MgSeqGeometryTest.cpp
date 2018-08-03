/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <libs/include/TSCTimer.h>
#include <multigrid/mgmesytec/MgSeqGeometry.h>
#include <test/TestBase.h>

class MgSeqGeometryTest : public TestBase {};

/** Test cases below */

TEST_F(MgSeqGeometryTest, IsWireIsGrid) {
  MgSeqGeometry mgdet;
  mgdet.add_bus(MgBusGeometry());

  for (int i = 0; i <= 79; i++) {
    ASSERT_TRUE(mgdet.isWire(0, i));
    ASSERT_FALSE(mgdet.isGrid(0, i));
  }

  for (int i = 80; i <= 119; i++) {
    ASSERT_FALSE(mgdet.isWire(0, i));
    ASSERT_TRUE(mgdet.isGrid(0, i));
  }

  ASSERT_FALSE(mgdet.isWire(0, 128));
  ASSERT_FALSE(mgdet.isGrid(0, 128));
}

#if 0
TEST_F(MgSeqGeometryTest, XZCoordinates) {
  MgSeqGeometry mgdet;
  int digitizer = 0;
  for (int xoffset = 0; xoffset < 4; xoffset++) {
    MESSAGE() << "Lower wires: " << xoffset * 16 << " to " << (xoffset * 16 + 15) << "\n";
    for (int zoffset = 0; zoffset < 16; zoffset++) {
      int channel = xoffset * 16 + zoffset;
      ASSERT_EQ(xoffset, mgdet.xcoord(digitizer, channel));
      ASSERT_EQ(-1, mgdet.ycoord(channel));
      ASSERT_EQ(zoffset , mgdet.zcoord(channel));
    }
  }

  for (int xoffset = 0; xoffset < 4; xoffset++) {
    MESSAGE() << "Upper wires: " << 64 + xoffset * 4 << " to " << (64 + xoffset * 4 + 3) << "\n";
    for (int zoffset = 0; zoffset < 4; zoffset++) {
      int channel = 64 + xoffset * 4 + zoffset;
      //MESSAGE() << "channel: " << channel << "\n";
      ASSERT_EQ(xoffset, mgdet.xcoord(digitizer, channel));
      ASSERT_EQ(-1 , mgdet.ycoord(channel));
      ASSERT_EQ(16 + zoffset, mgdet.zcoord(channel));
    }
  }
}

TEST_F(MgSeqGeometryTest, YCoordinates) {
  MgSeqGeometry mgdet;
  int digitizer = 0;
  for (int channel = 80; channel < 127; channel++) {
    ASSERT_EQ(-1, mgdet.xcoord(digitizer, channel));
    ASSERT_EQ(channel - 80 , mgdet.ycoord(channel));
    ASSERT_EQ(-1, mgdet.zcoord(channel));
  }
}

#endif

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
