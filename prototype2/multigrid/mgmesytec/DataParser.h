/** Copyright (C) 2016-2018 European Spallation Source */
//===----------------------------------------------------------------------===//
///
/// \file
/// Class to parse detector readout for multigrid via
/// sis3153 / Mesytec digitizer
///
//===----------------------------------------------------------------------===//

#pragma once
#include <common/EV42Serializer.h>
#include <logical_geometry/ESSGeometry.h>
#include <multigrid/mgmesytec/Vmmr16Parser.h>
#include <multigrid/mgmesytec/HitFile.h>

class MesytecData {
public:
  enum class error { OK = 0, ESIZE, EHEADER, EUNSUPP };

  /// \brief if it looks like a constructor...
  MesytecData(MgEFU mg_efu, std::shared_ptr<ReadoutSerializer> s, bool spoof_ht,
              std::shared_ptr<MGHitFile> dump = nullptr);

  ~MesytecData() = default;

  /// \todo document
  uint32_t getPixel();

  /// \todo document
  uint32_t getTime();

  /// \brief parse a binary payload buffer, return number of data element
  error parse(const char *buffer, int size, EV42Serializer &EV42Serializer);

  /// Statistics updated by parse()
  MgStats stats;

  uint64_t RecentPulseTime{0};

private:
  VMMR16Parser vmmr16Parser;

  // \todo deduce this from mappings
  ESSGeometry Geometry{36, 40, 20, 1};

  std::shared_ptr<MGHitFile> dumpfile;
};
