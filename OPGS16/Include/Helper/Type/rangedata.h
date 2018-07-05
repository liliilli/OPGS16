#ifndef OPGS16_HELPER_TYPE_RANGE_DATA_H
#define OPGS16_HELPER_TYPE_RANGE_DATA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/rangedata.h
///
/// @brief Restriction template struct type which has range.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-05 Create file.
///

namespace opgs16 {

template <typename TType>
struct DRangeData {
  TType from;
  TType inclusive_to;

  constexpr explicit DRangeData(const TType& from, const TType& inclusive_to) :
      from{from}, inclusive_to{inclusive_to} {
    if (this->from > this->inclusive_to) throw 0;
  }
  constexpr ~DRangeData() = default;
  constexpr DRangeData(const DRangeData&) = default;
  constexpr DRangeData& operator=(const DRangeData&) = default;
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_RANGE_DATA_H