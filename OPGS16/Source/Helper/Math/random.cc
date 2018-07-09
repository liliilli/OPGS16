
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Math/random.cc
/// @author Jongmin Yun
///
/// @log
/// 2018-07-01 Create file.
///

/// Header file
#include <Helper/Math/random.h>

#include <cmath>

#include <chrono>
#include <limits>
#include <random>

#include <Helper/Type/vector2.h>
#include <Helper/Type/vector3.h>

//!
//! Data
//!

namespace {

std::random_device rd;
std::mt19937 rng_device(rd());

std::uniform_int_distribution<int> int_distribution;
std::uniform_real_distribution<float> real_distribution;
}

//!
//! Implemenation
//!

namespace opgs16::random {

int32_t RandomInteger() {
  return int_distribution(rng_device);
}

float RandomFloat() {
  return real_distribution(rng_device) * 200.f - 100.f;
}

int32_t RandomPositiveInteger() {
  const int32_t value = int_distribution(rng_device);

  if (value == std::numeric_limits<int32_t>::lowest()) return 0;
  if (value < 0) return -value;
  return value;
}

int32_t RandomNegativeInteger() {
  const int32_t value = int_distribution(rng_device);

  if (value > 0) return -value;
  return value;
}

float RandomPositiveFloat() {
  const float value = real_distribution(rng_device) * 100.f;

  if (value < 0.f) return -value;
  return value;
}

float RandomNegativeFloat() {
  const float value = real_distribution(rng_device) * 100.f;

  if (value > 0.f) return -value;
  return value;
}

int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to) {
  auto result = int_distribution(rng_device);
  result %= (inclusive_to - from + 1);
  result += from;

  return result;
}

float RandomFloatRange(const float from, const float prior_to) {
  auto result = RandomFloat();
  result = std::fmod(result, prior_to - from);
  result += from;

  return result;
}

DVector2 RandomVector2Length(float length) {
  DVector2 result;
  float squared_length;

  do {
    result.x = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.y = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length > std::pow(length * 1.75f, 2) ||
         squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DVector3 RandomVector3Length(float length) {
  DVector3 result;
  float squared_length;

  do {
    result.x = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.y = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.z = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length < std::pow(length * 1.5f, 2) ||
         squared_length > std::pow(length * 0.5f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DVector2 RandomVector2Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform: return {}; break;
  case ERandomPolicy::Discrete: return {}; break;
  }
}

DVector3 RandomVector3Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform: return {}; break;
  case ERandomPolicy::Discrete: return {}; break;
  }
}

} /// ::opgs16::random namespace