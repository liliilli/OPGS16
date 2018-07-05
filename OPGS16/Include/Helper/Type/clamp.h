#ifndef OPGS16_HELPER_TYPE_CLAMP_H
#define OPGS16_HELPER_TYPE_CLAMP_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/clamp.h
///
/// @brief Restriction template struct type which has range.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-05 Create file.
///

#include <Helper/Math/math.h>
#include <Helper/Type/rangedata.h>
#include <spdlog/fmt/fmt.h>

namespace opgs16 {

template <typename TType>
class DClamp {
public:
  constexpr explicit DClamp(
      const TType& value,
      const TType& from,
      const TType& inclusive_to) noexcept {
    m_range = {from, inclusive_to};
    m_value = math::Clamp(value, from, inclusive_to);
  }

  constexpr explicit DClamp(const TType& from,
                            const TType& inclusive_to) noexcept :
      DClamp{from, from, inclusive_to} {}

  constexpr explicit DClamp(const TType& value,
                            const DRangeData<TType>& range) noexcept :
      DClamp{value, range.from, range.inclusive_to} {};

  constexpr explicit DClamp(const DRangeData<TType>& range) noexcept :
      DClamp{range.from, range.from, range.inclusive_to} {};

  constexpr DClamp() :
      DClamp{std::numeric_limits<TType>::lowest(),
             std::numeric_limits<TType>::max()} {}
  ~DClamp() = default;

  constexpr DClamp(const DClamp& instance) = default;
  constexpr DClamp& operator=(const DClamp& instance) = default;

  constexpr const DRangeData<TType>& GetRange() const noexcept {
    return m_range;
  }

  constexpr DClamp& operator=(const TType& value) noexcept {
    m_value = math::Clamp(value, m_range);
    return *this;
  }

  constexpr TType operator()() const noexcept {
    return m_value;
  }

  constexpr friend bool operator==(const DClamp<TType>& lhs,
                                   const TType& value) noexcept {
    return lhs.m_value == value;
  }

private:
  DRangeData<TType> m_range;
  TType m_value;
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_CLAMP_H
