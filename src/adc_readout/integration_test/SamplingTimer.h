/* Copyright (C) 2017-2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#pragma once

#include "../AdcTimeStamp.h"
#include <functional>

class SamplingTimer {
public:
  explicit SamplingTimer(std::function<void(TimeStamp const &)> OnTimer);
  virtual ~SamplingTimer() = default;
  virtual void start() = 0;
  virtual void stop() = 0;

  virtual void genSamplesAndEnqueueSend();

  std::function<void(TimeStamp const &)> SamplingFunc;
};
