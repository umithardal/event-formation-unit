/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <common/Producer.h>
#include <cstring>
#include <common/HistSerializer.h>
#include <test/TestBase.h>

#define MAX_STRIP_VAL_TEST 5000

class HistSerializerTest : public TestBase {
  virtual void SetUp() {
    for (size_t i = 0; i < hists.x_strips_hist.size(); i++) {
      hists.x_strips_hist[i] = i;
      hists.y_strips_hist[i] = MAX_STRIP_VAL_TEST - i;
    }
  }

  virtual void TearDown() {}

protected:
  Hists hists {MAX_STRIP_VAL_TEST, MAX_STRIP_VAL_TEST};
  char flatbuffer[1024 * 1024 * 5];
  Producer producer{"localhost:9092", "test_monitor"};
};

TEST_F(HistSerializerTest, Serialize) {
  HistSerializer histfb(hists.needed_buffer_size(), producer);
  auto len = histfb.produce(hists);
  ASSERT_TRUE(len >= hists.needed_buffer_size());
}

// \todo reimplement this test with a verifier?
/*
TEST_F(HistSerializerTest, DeSerialize) {
  HistSerializer histfb(hists.needed_buffer_size(), producer);

  auto length = histfb.produce((hists);

  memcpy(flatbuffer, buffer, length);
  auto monitor = GetMonitorMessage(flatbuffer);
  auto dtype = monitor->data_type();
  ASSERT_EQ(dtype, DataField::GEMHist);

  auto hist = static_cast<const GEMHist *>(monitor->data());
  auto xdat = hist->xstrips();
  auto ydat = hist->ystrips();
  ASSERT_EQ(xdat->size(), hists.x_strips_hist.size());
  ASSERT_EQ(ydat->size(), hists.y_strips_hist.size());

  for (size_t i = 0; i < hists.x_strips_hist.size(); i++) {
    ASSERT_EQ((*xdat)[i], i);
    EXPECT_EQ((*ydat)[i], MAX_STRIP_VAL_TEST - i);
  }
}
*/

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
