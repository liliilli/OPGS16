#ifndef OPGS16_HELPER_TYPE_VECTORI2_H
#define OPGS16_HELPER_TYPE_VECTORI2_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/vectori2.h
///
/// @brief
/// Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-30 Create file.
///

#include <cstdint>

#include <glm/glm.hpp>

/// ::opgs16::DVector2 (float)
#include <Helper/Type/vector2.h>
#include <Helper/Type/vector3.h>

namespace opgs16 {

///
/// @struct DVectorInt2
/// @brief int32 type 2-element vector struct.
///
struct DVectorInt2 {
  int32_t x = 0;
  int32_t y = 0;

  DVectorInt2() = default;
  ~DVectorInt2() = default;

  DVectorInt2(const int32_t x, const int32_t y) noexcept :
      x{x}, y{y} {};

  explicit DVectorInt2(const int32_t value) noexcept :
      x{value}, y{value} {}

  DVectorInt2(const DVectorInt2&) = default;
  DVectorInt2& operator=(const DVectorInt2& value) = default;

  DVectorInt2(DVectorInt2&&) = default;
  DVectorInt2& operator=(DVectorInt2&&) = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  explicit DVectorInt2(const glm::ivec2& value) noexcept :
      x{value.x}, y{value.y} {};

  DVectorInt2& operator=(const glm::ivec2& value) noexcept {
    x = value.x;
    y = value.y;
    return *this;
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator DVector2() const noexcept {
    return DVector2{static_cast<float>(x), static_cast<float>(y)};
  }

  explicit operator DVector3() const noexcept {
    return DVector3{static_cast<float>(x), static_cast<float>(y), 0.f};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief Return one-dimensional data chunk of DVectorInt2.
  ///
  std::array<int32_t, 2> Data() const noexcept {
    return {x, y};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Float type length of this DVectorInt2.
  ///
  float GetLength() const noexcept {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Float squared length of this DVectorInt2.
  ///
  float GetSquareLength() const noexcept {
    return static_cast<float>(x * x + y * y);
  }

  //!
  //! Operators
  //!

  friend DVectorInt2 operator+(DVectorInt2 lhs, const DVectorInt2& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
  }

  friend DVectorInt2 operator-(DVectorInt2 lhs, const DVectorInt2& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
  }

  ///
  /// DVectorInt2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVectorInt2 operator*(DVectorInt2 lhs, const int32_t rhs) noexcept {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVectorInt2, element multiplication happens.
  ///
  friend DVectorInt2 operator*(DVectorInt2 lhs, const DVectorInt2& rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVectorInt2 operator/(DVectorInt2 lhs, const int32_t rhs) noexcept {
    if (rhs == 0) {
      PUSH_LOG_CRITICAL_EXT("DVectorInt2 could not be divided by {0}.", rhs);
    }
    else {
      lhs.x = static_cast<int>(static_cast<float>(lhs.x) / rhs);
      lhs.y = static_cast<int>(static_cast<float>(lhs.y) / rhs);
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVectorInt2 operator/(DVectorInt2 lhs, const DVectorInt2& rhs) noexcept {
    if (rhs.x == 0 || rhs.y == 0) {
      PUSH_LOG_CRITICAL_EXT(
          "DVectorInt2 could not be devided by 0 included DVectorInt2, ({0}, {1})",
          rhs.x, rhs.y);
    }
    else {
      lhs.x = static_cast<int>(static_cast<float>(lhs.x) / rhs.x);
      lhs.y = static_cast<int>(static_cast<float>(lhs.y) / rhs.y);
    }

    return lhs;
  }

  DVectorInt2& operator+=(const DVectorInt2& value) noexcept {
    this->x += value.x;
    this->y += value.y;
    return *this;
  }

  DVectorInt2& operator-=(const DVectorInt2& value) noexcept {
    this->x -= value.x;
    this->y -= value.y;
    return *this;
  }

  DVectorInt2& operator*=(const int32_t value) noexcept {
    this->x *= value;
    this->y *= value;
    return *this;
  }

  DVectorInt2& operator*=(const DVectorInt2& value) noexcept {
    this->x *= value.x;
    this->y *= value.y;
    return *this;
  }

  ///
  /// If lhs and rhs are DVectorInt2, element multiplication happens.
  ///
  DVectorInt2& operator/=(const int32_t value) noexcept {
    if (value == 0) {
      PUSH_LOG_CRITICAL_EXT("DVectorInt2 could not be divided by {0}.", value);
    }
    else {
      this->x = static_cast<int>(static_cast<float>(this->x) / value);
      this->y = static_cast<int>(static_cast<float>(this->y) / value);
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVectorInt2& operator/=(const DVectorInt2& value) noexcept {
    if (value.x == 0 || value.y == 0) {
      PUSH_LOG_CRITICAL_EXT(
          "DVectorInt2 could not be devided by 0 included DVectorInt2, ({0}, {1})",
          value.x, value.y);
    }
    else {
      this->x = static_cast<int>(static_cast<float>(this->x) / value.x);
      this->y = static_cast<int>(static_cast<float>(this->y) / value.y);
    }

    return *this;
  }

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return lhs.GetSquareLength() == rhs.GetSquareLength();
  }

private:
  friend bool operator<(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return rhs < lhs;
  }

  friend bool operator<=(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return !(lhs < rhs);
  }

  //!
  //! Static functions
  //!
public:

  ///
  /// @brief Do dot product of (x, y) R^2 vector.
  /// @return Dot product float value.
  ///
  static int32_t Dot(const DVectorInt2& lhs, const DVectorInt2& rhs) noexcept {
    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVectorInt2 CompMaxLength(const DVectorInt2& lhs,
                                   const DVectorInt2& rhs) noexcept {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVectorInt2 CompMinLength(const DVectorInt2& lhs,
                                   const DVectorInt2& rhs) noexcept {
    return (lhs < rhs) ? lhs : rhs;
  }
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_VECTORI2_H