#include <precompiled.h>
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
#include <Phitos/Dbg/assert.h>

//!
//! Data
//!

namespace {

std::random_device rd;
std::mt19937 rng_device(rd());

std::uniform_int_distribution<int>    int_distribution;
std::uniform_real_distribution<float> real_distribution{
  -1'000'000'000.f,
  1'000'000'000.f
};

}

//!
//! Implemenation
//!

namespace opgs16::random {

int32_t RandomInteger() {
  return int_distribution(rng_device);
}

float RandomFloat() {
  return real_distribution(rng_device);
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
  const float value = real_distribution(rng_device);

  if (value < 0.f) return -value;
  return value;
}

float RandomNegativeFloat() {
  const float value = real_distribution(rng_device);

  if (value > 0.f) return -value;
  return value;
}

int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to) {
  PHITOS_ASSERT(from <= inclusive_to, "");

  auto result = int_distribution(rng_device);
  result %= (inclusive_to - from + 1);
  result += from;

  return result;
}

float RandomFloatRange(const float from, const float prior_to) {
  PHITOS_ASSERT(from <= prior_to, "");

  const std::uniform_real_distribution<float> rng{from, prior_to};
  return rng(rng_device);
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
  while (squared_length > std::pow(length * 1.75f, 2) ||
         squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DVector2 RandomVector2Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform:
    return RandomVector2Length(1.f) * RandomFloatRange(from, prior_to);
  case ERandomPolicy::Discrete:
    return {};
  default: return {};
  }
}

DVector3 RandomVector3Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform:
    return RandomVector3Length(1.f) * RandomFloatRange(from, prior_to);
  case ERandomPolicy::Discrete:
    return {};
  default: return {};
  }
}

} /// ::opgs16::random namespace