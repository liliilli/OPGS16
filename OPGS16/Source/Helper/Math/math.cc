#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Math/math.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-28 Create file
///

/// Header file
#include <Helper/Math/math.h>

#include <cfenv>

#include <limits>

namespace opgs16::math {

float Lerp(float lhs, float rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<float>::lowest();
  }
  return result;
}

double Lerp(double lhs, double rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<double>::lowest();
  }
  return result;
}

DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}

DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}

DVector2 GetQuadBezierCurvePoint(const DVector2& lhs,
                                 const DVector2& rhs,
                                 const DVector2& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset),
              Lerp(control, rhs, offset),
              offset);
}

DVector3 GetQuadBezierCurvePoint(const DVector3& lhs,
                                 const DVector3& rhs,
                                 const DVector3& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset),
              Lerp(control, rhs, offset),
              offset);
}

} /// ::opgs16::math namespace