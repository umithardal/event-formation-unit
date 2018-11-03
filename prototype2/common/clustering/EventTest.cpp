/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <cmath>
#include <common/clustering/Event.h>
#include <string>
#include <test/TestBase.h>
#include <unistd.h>

class EventTest : public TestBase {
protected:
  Hit e;
  Multiblade::Event event;
  virtual void SetUp() {  }
  virtual void TearDown() {  }
};

TEST_F(EventTest, Insert) {
  event.insert_hit(e);
  EXPECT_EQ(event.x.hit_count(), 1);
  e.plane = 1;
  event.insert_hit(e);
  EXPECT_EQ(event.y.hit_count(), 1);
}

TEST_F(EventTest, Empty) {
  EXPECT_TRUE(event.empty());
  event.insert_hit(e);
  EXPECT_FALSE(event.empty());
}

TEST_F(EventTest, Merge) {
  Cluster x;
  e.plane = 0;
  x.insert_hit(e);
  x.insert_hit(e);
  event.merge(x);
  EXPECT_FALSE(event.empty());
  EXPECT_EQ(x.hit_count(), 0);
  EXPECT_EQ(event.x.hit_count(), 2);
}

TEST_F(EventTest, MergeTwice) {
  Cluster x;
  e.plane = 0;

  x.insert_hit(e);
  x.insert_hit(e);
  event.merge(x);
  EXPECT_EQ(event.x.hit_count(), 2);

  x.insert_hit(e);
  x.insert_hit(e);
  x.insert_hit(e);
  event.merge(x);
  EXPECT_EQ(event.x.hit_count(), 5);
}

TEST_F(EventTest, MergeXY) {
  Cluster x, y;

  e.plane = 0;
  x.insert_hit(e);
  x.insert_hit(e);
  event.merge(x);

  e.plane = 1;
  y.insert_hit(e);
  y.insert_hit(e);
  y.insert_hit(e);
  event.merge(y);
  EXPECT_EQ(event.x.hit_count(), 2);
  EXPECT_EQ(event.y.hit_count(), 3);
}

TEST_F(EventTest, TimeSpan) {
  Cluster x, y;

  e.plane = 0;
  e.time = 3; x.insert_hit(e);
  e.time = 7; x.insert_hit(e);
  event.merge(x);

  e.plane = 1;
  e.time = 5; y.insert_hit(e);
  e.time = 1; y.insert_hit(e);
  event.merge(y);
  EXPECT_EQ(event.time_end(), 7);
  EXPECT_EQ(event.time_start(), 1);
  EXPECT_EQ(event.time_span(), 6);
}


TEST_F(EventTest, InsertInvalid) {
  e.plane = 0;
  event.insert_hit(e);
  e.plane = 1;
  event.insert_hit(e);
  e.plane = 2;
  event.insert_hit(e);
  EXPECT_EQ(2, event.x.hit_count() + event.y.hit_count());
}

TEST_F(EventTest, DebugPrint) {
  MESSAGE() << "This is not a test, just calling debug print function\n";
  event.insert_hit(e);
  e.plane = 1;
  event.insert_hit(e);
  auto debugstr = event.debug();
  MESSAGE() << debugstr << "\n";
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
