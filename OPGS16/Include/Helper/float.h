#ifndef OPGS16_HELPER_FLOAT_H
#define OPGS16_HELPER_FLOAT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

namespace opgs16 {

inline constexpr bool __ApproximateEqual(float lhs, float rhs,
                                         const float precision) noexcept {
  const auto diff = lhs - rhs;

  if (diff > precision) return false;
  if (diff < -precision) return false;

  return true;
}

} /// ::opgs16::namespace

#endif /// OPGS16_HELPER_FLOAT_H