/* Copyright (C) 2018 European Spallation Source, ERIC. See LICENSE file */
//===----------------------------------------------------------------------===//
///
/// \file OverlapMatcher.h
/// \brief OverlapMatcher class definition
///
//===----------------------------------------------------------------------===//

#pragma once

#include <common/clustering/AbstractMatcher.h>

/// \class OverlapMatcher OverlapMatcher.h
/// \brief Matcher implementation that joins clusters into events
///         if they overlap in time.

class OverlapMatcher : public AbstractMatcher {
public:
  /// \brief OverlapMatcher constructor
  /// \sa AbstractMatcher
  OverlapMatcher(uint64_t latency);

  /// \brief OverlapMatcher constructor
  /// \sa AbstractMatcher
  OverlapMatcher(uint64_t latency, uint8_t plane1, uint8_t plane2);

  /// \brief Match queued up clusters into events.
  ///         Clusters that overlap in time are joined into events.
  /// \param flush if all queued clusters should be matched regardless of
  ///        latency considerations
  void match(bool flush) override;
};
