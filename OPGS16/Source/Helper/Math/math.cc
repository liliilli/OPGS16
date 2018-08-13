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

#include <Helper/Type/vectori2.h>
#include <Helper/Type/vectori3.h>

namespace {

template <typename TType>
constexpr TType rad_to_deg_offset = 180 / opgs16::math::pi<TType>;

template <typename TType>
constexpr TType deg_to_rad_offset = opgs16::math::pi<TType> / 180;

} /// unnamed namespace

namespace opgs16::math {

bool IsAllZero(const opgs16::DVector2& vector) noexcept {
  if (!IsNearlyEqual(vector.x, 0.f)) return false;
  if (!IsNearlyEqual(vector.y, 0.f)) return false;
  return true;
}

bool IsAllZero(const opgs16::DVector3& vector) noexcept {
  if (!IsNearlyEqual(vector.x, 0.f)) return false;
  if (!IsNearlyEqual(vector.y, 0.f)) return false;
  if (!IsNearlyEqual(vector.z, 0.f)) return false;
  return true;
}

bool IsAllZero(const opgs16::DVectorInt2& vector) noexcept {
  if (vector.x != 0) return false;
  if (vector.y != 0) return false;
  return true;
}

bool IsAllZero(const opgs16::DVectorInt3& vector) noexcept {
  if (vector.x != 0) return false;
  if (vector.y != 0) return false;
  if (vector.z != 0) return false;
  return true;
}

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

TMinMaxResult<float> GetMinMax(const opgs16::DVector2& vector) noexcept {
  if (vector.x < vector.y) {
    return {vector.x, vector.y};
  }
  else {
    return {vector.y, vector.x};
  }
}

TMinMaxResult<float> GetMinMax(const opgs16::DVector3& vector) noexcept {
  auto min = vector.x;
  auto max = min;

  if (vector.y < min) min = vector.y;
  if (vector.y > max) max = vector.y;
  if (vector.z < min) min = vector.z;
  if (vector.z > max) max = vector.z;
  return {min, max};
}

TMinMaxResult<int32_t> GetMinMax(const opgs16::DVectorInt2& vector) noexcept {
  if (vector.x < vector.y) {
    return {vector.x, vector.y};
  }
  else {
    return {vector.y, vector.x};
  }
}

TMinMaxResult<int32_t> GetMinMax(const opgs16::DVectorInt3& vector) noexcept {
  auto min = vector.x;
  auto max = min;

  if (vector.y < min) min = vector.y;
  if (vector.y > max) max = vector.y;
  if (vector.z < min) min = vector.z;
  if (vector.z > max) max = vector.z;
  return {min, max};
}

float GetRotationAngle(float angle_value) noexcept {
  const float angle = std::fmod(angle_value, 360.f);
  return (angle < 0.f) ? angle + 360.f : angle;
}

double GetRotationAngle(double angle_value) noexcept {
  const double angle = std::fmod(angle_value, 360.0);
  return (angle <= 0.0) ? angle + 360.0 : angle;
}

float GetRotationAngleRadian(float angle_value) noexcept {
  const float angle = std::fmod(angle_value, math::pi<float> * 2);
  return (angle <= 0.f) ? angle + (pi<float> * 2) : angle;
}

double GetRotationAngleRadian(double angle_value) noexcept {
  const double angle = std::fmod(angle_value, math::pi<double> * 2);
  return (angle <= 0.0) ? angle + (pi<float> * 2) : angle;
}

float RadToDeg(float radian) noexcept {
  return GetRotationAngle(radian * rad_to_deg_offset<float>);
}

double RadToDeg(double radian) noexcept {
  return GetRotationAngle(radian * rad_to_deg_offset<double>);
}

float DegToRad(float degree) noexcept {
  return GetRotationAngleRadian(degree * deg_to_rad_offset<float>);
}

double DegToRad(double degree) noexcept {
  return GetRotationAngleRadian(degree * deg_to_rad_offset<double>);
}

} /// ::opgs16::math namespace