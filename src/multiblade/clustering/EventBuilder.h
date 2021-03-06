/* Copyright (C) 2016-2018 European Spallation Source, ERIC. See LICENSE file */

#pragma once

#include <multiblade/caen/Readout.h>

#include <common/reduction/clustering/GapClusterer.h>
#include <common/reduction/matching/GapMatcher.h>

// \todo put these into namespace scope
static constexpr uint64_t latency{125}; // 2us @ 16ns/tick (2000/16)
static constexpr uint64_t coordgap{1};  // allow no gaps between channels
static constexpr uint64_t timegap{70};  // expect readouts in a plane to be at least this close

namespace Multiblade {

class EventBuilder {
public:
  EventBuilder();

  // \todo pass by rvalue?
  void insert(Hit hit);

  void flush();

  void clear();

  HitVector p0, p1;

  // \todo parametrize
  GapClusterer c0{timegap, coordgap}, c1{timegap, coordgap};

  // \todo parametrize
  GapMatcher matcher{latency, 0, 1};
};

}
