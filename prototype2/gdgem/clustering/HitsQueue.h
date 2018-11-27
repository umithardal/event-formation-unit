/* Copyright (C) 2016-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#pragma once

#include <gdgem/clustering/HitContainer.h>
#include <gdgem/srs/SRSTime.h>

namespace Gem {

class HitsQueue {
public:
  HitsQueue(SRSTime Time);
  void store(uint8_t plane, uint16_t strip, uint16_t adc,
             float chipTime, uint64_t trigger_time);

  /// \todo add a flush flag here
  void sort_and_correct();

  const HitContainer &hits() const;

  void clear();

private:
  // tripple buffer

  HitContainer hitsOut;

  SRSTime pTime;
};

}
