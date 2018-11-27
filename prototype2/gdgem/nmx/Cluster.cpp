/** Copyright (C) 2016, 2017 European Spallation Source ERIC */

#include <gdgem/nmx/Cluster.h>
#include <cmath>
#include <set>
#include <sstream>
#include <algorithm>

#include <common/Trace.h>
//#undef TRC_LEVEL
//#define TRC_LEVEL TRC_L_DEB

namespace Gem {

void Cluster::insert_hit(const Hit &e) {
  if (hits.empty()) {
    plane_id = e.plane;
    time_start = time_end = e.time;
    strip_start = strip_end = e.coordinate;
  }

  // If plane identities don't match, invalidate
  /// \todo this needs more testing
  if (plane_id != e.plane) {
    plane_id = -1;
  }

  hits.push_back(e);
  adc_sum += e.weight;
  strip_mass += e.weight * e.coordinate;
  time_mass += e.weight * e.time;
  time_start = std::min(time_start, e.time);
  time_end = std::max(time_end, e.time);
  strip_start = std::min(strip_start, e.coordinate);
  strip_end = std::max(strip_end, e.coordinate);
}

bool Cluster::empty() const
{
  return hits.empty();
}

uint64_t Cluster::time_span() const {
  return time_end - time_start;
}

uint16_t Cluster::strip_span() const {
  if (hits.empty()) {
    return 0;
  }
  return (strip_end - strip_start) + 1u;
}

double Cluster::strip_center() const {
  return strip_mass / adc_sum;
}

double Cluster::time_center() const {
  return time_mass / adc_sum;
}

/// \todo make work with doubles (or not, if we decimate timestamps?)
void Cluster::analyze(bool weighted, uint16_t max_timebins,
                      uint16_t max_timedif) {
  if (hits.empty()) {
    return;
  }

  std::sort(hits.begin(), hits.end(),
      [](const Hit &c1, const Hit &c2) {
    return c1.time < c2.time;
  });

  double center_sum{0};
  double center_count{0};
  int16_t lspan_min = std::numeric_limits<int16_t>::max();
  int16_t lspan_max = std::numeric_limits<int16_t>::min();
  int16_t uspan_min = std::numeric_limits<int16_t>::max();
  int16_t uspan_max = std::numeric_limits<int16_t>::min();
  uint64_t earliest = std::min(time_start, time_end - static_cast<uint64_t>(max_timedif));
  std::set<uint64_t> timebins;
  for (auto it = hits.rbegin(); it != hits.rend(); ++it) {
    auto e = *it;
    if (e.time == time_end) {
      if (weighted) {
        center_sum += (e.coordinate * e.weight);
        center_count += e.weight;
      } else {
        center_sum += e.coordinate;
        center_count++;
      }
      lspan_min = std::min(lspan_min, static_cast<int16_t>(e.coordinate));
      lspan_max = std::max(lspan_max, static_cast<int16_t>(e.coordinate));
    }
    if ((e.time >= earliest) && ((max_timebins > timebins.size()) || (timebins.count(e.time)))) {
      timebins.insert(e.time);
      uspan_min = std::min(uspan_min, static_cast<int16_t>(e.coordinate));
      uspan_max = std::max(uspan_max, static_cast<int16_t>(e.coordinate));
    } else {
      break;
    }
  }

  XTRACE(PROCESS, DEB, "center_sum=%f center_count=%f", center_sum,
         center_count);

  utpc_center = center_sum / center_count;
  uncert_lower = lspan_max - lspan_min + 1;
  uncert_upper = uspan_max - uspan_min + 1;
}

uint32_t Cluster::utpc_center_rounded() const {
  return static_cast<uint32_t>(std::round(utpc_center));
}

void Cluster::merge(Cluster &other) {
  if (other.hits.empty()) {
    return;
  }

  if (hits.empty()) {
    *this = std::move(other);
    return;
  }

  if (other.plane_id != plane_id) {
    return;
  }

  hits.reserve( hits.size() + other.hits.size() ); // preallocate memory
  hits.insert( hits.end(), other.hits.begin(), other.hits.end() );

  adc_sum += other.adc_sum;
  strip_mass += other.strip_mass;
  time_mass += other.time_mass;
  time_start = std::min(time_start, other.time_start);
  time_end = std::max(time_end, other.time_end);
  strip_start = std::min(strip_start, other.strip_start);
  strip_end = std::max(strip_end, other.strip_end);
}

/// \todo Unit tests for this
uint64_t Cluster::time_overlap(const Cluster &other) const {
  auto latest_start = std::max(other.time_start, time_start);
  auto earliest_end = std::min(other.time_end, time_end);
  if (latest_start > earliest_end) {
    return 0;
  }
  return earliest_end - latest_start;
}

/// \todo Precision of comparisons
/// \todo Comments or helper methods
bool Cluster::time_touch(const Cluster &other) const {
  if ((other.time_start == other.time_end) &&
      (time_start < other.time_end) && (other.time_end < time_end)) {
    return true;
  }
  if ((time_start == time_end) &&
      (other.time_start < time_end) && (time_end < other.time_end)) {
    return true;
  }
  return ((time_start == other.time_end) ||
      (time_end == other.time_start));
}

std::string Cluster::debug() const {
  std::stringstream ss;
  ss << "    C=" << utpc_center << " +-" << uncert_lower << " (+-" << uncert_upper
     << ")\n";
  ss << "    T=(" << time_start << "-" << time_end << ")"
     << " integral=" << adc_sum << "\n";
  //  for (const auto& e : entries)
  //    ss << e.debug() << "\n";
  return ss.str();
}

}
