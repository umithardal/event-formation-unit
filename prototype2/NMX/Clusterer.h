/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

/** @file
 *
 *  @brief Class for NMX event clustering
 */

#pragma once

#include <NMX/EventNMX.h>
#include <NMX/Eventlet.h>
#include <map>

class Clusterer {
public:

  /** @brief create an NMX event clusterer
   * @param min_time_span minimum timebins that constitute one event
   */
  Clusterer(uint64_t min_time_span);

  /** @brief add eventlet onto the clustering stack
   * @param eventlet with valid timestamp and non-zero adc value
   */
  void insert(const Eventlet &eventlet);

  /** @brief indicates if there is an event ready for clustering
   */
  bool event_ready() const;

  /** @brief returns a clustered event (if one is ready, else empty event)
   */
  EventNMX get_event();

private:
  std::multimap<uint64_t, Eventlet> backlog_; //stack of chronologically ordered events
  uint64_t min_time_span_{1};
};
