/** Copyright (C) 2019 European Spallation Source, ERIC. See LICENSE file **/
//===----------------------------------------------------------------------===//
///
/// \file MGAnalyzer.h
/// \brief MGAnalyzer class definition
///
//===----------------------------------------------------------------------===//
#pragma once

#include <common/reduction/analysis/AbstractAnalyzer.h>
#include <multigrid/geometry/ModuleGeometry.h>

/// \class MGAnalyzer MGAnalyzer.h
/// \brief MGAnalyzer Implements a strategy for analyzing Multi-grid events.
///        In general this means averaging the one ore more hits with the highest
///        weight value. Any hits below the highest weight are ignored. One of the
///        dimensions is assumed to be a convolution of two dimensions, and
///        therefore a ModuleGeometry definition is needed to extract the two
///        coordinate components. Produces 3-dimensional events.

// \todo document this better

class MGAnalyzer : public AbstractAnalyzer {
public:
  /// \param weighted if strategy should use weighted average for coordinate calculation
  void weighted(bool weighted);

  /// \param geom sets the ModuleGeometry definition for converting Wires to X and Z
  void set_geometry(const Multigrid::ModuleGeometry& geom);

  /// \returns current ModuleGeometry definition
  Multigrid::ModuleGeometry geometry() const;

  /// \brief analyzes event in both planes
  ReducedEvent analyze(Event&) const override;

  /// \brief analyzes cluster as wires
  void analyze_wires(Cluster& cluster, ReducedHit& x, ReducedHit& z) const;

  /// \brief analyzes cluster as grids
  ReducedHit analyze_grids(Cluster& cluster) const;

  /// \brief prints info for debug purposes
  std::string debug(const std::string& prepend) const override;

  mutable size_t stats_used_hits{0};

private:
  bool weighted_{true};

  // \todo use pre-generated look-up table
  Multigrid::ModuleGeometry geometry_;
};
