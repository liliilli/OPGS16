#ifndef OPGS16_HELPER_MATH_MATH_H
#define OPGS16_HELPER_MATH_MATH_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/math.h
///
/// @brief Restriction template struct type which has range.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-05 Create file.
///

#include <Helper/Type/rangedata.h>

namespace opgs16::math {

template <typename TType>
constexpr TType Clamp(const TType& value,
                      const TType& from, const TType& inclusive_to) noexcept {
  return (value > from ? (value < inclusive_to ? value : inclusive_to) : from);
}

template <typename TType>
constexpr TType Clamp(const TType& value,
                      const DRangeData<TType>& range) noexcept {
  return
  (value > range.from
    ? (value < range.inclusive_to ?
      value :
      range.inclusive_to) :
    range.from );
}

///
/// @brief Check float lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two floating points.
///
constexpr bool IsNearlyEqual(const float lhs, const float rhs,
                             const float error_tolerance = 0.0001f) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

///
/// @brief Check double lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two double points.
///
constexpr bool IsNearlyEqual(const double lhs, const double rhs,
                             const double error_tolerance = 0.0001) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) < error_tolerance;
}

} /// ::opgs16::math namespace

#endif /// OPGS16_HELPER_MATH_MATH_H