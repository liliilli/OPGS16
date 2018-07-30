#ifndef OPGS16_HELPER_TYPE_VECTORI3_H
#define OPGS16_HELPER_TYPE_VECTORI3_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/vectori3.h
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

/// ::opgs16::DVector3 (float)
#include <Helper/Type/vector3.h>

namespace opgs16 {

///
/// @struct DVectorInt3
/// @brief int32 type 3-element vector struct.
///
struct DVectorInt3 {
  int32_t x = 0;
  int32_t y = 0;
  int32_t z = 0;

  DVectorInt3() = default;
  ~DVectorInt3() = default;

  DVectorInt3(const int32_t x, const int32_t y, const int32_t z) noexcept :
      x{x}, y{y}, z{z} {};

  explicit DVectorInt3(const int32_t value) noexcept :
      x{value}, y{value}, z{value} {}

  DVectorInt3(const DVectorInt3&) = default;
  DVectorInt3& operator=(const DVectorInt3& value) = default;

  DVectorInt3(DVectorInt3&&) = default;
  DVectorInt3& operator=(DVectorInt3&&) = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  explicit DVectorInt3(const glm::ivec2& value) noexcept :
      x{value.x}, y{value.y}, z{0} {};

  explicit DVectorInt3(const glm::ivec3& value) noexcept :
      x{value.x}, y{value.y}, z{value.z} {};

  DVectorInt3& operator=(const glm::ivec2& value) noexcept {
    x = value.x;
    y = value.y;
    z = 0;
    return *this;
  }

  DVectorInt3& operator=(const glm::ivec3& value) noexcept {
    x = value.x;
    y = value.y;
    z = value.z;
    return *this;
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator DVector3() const noexcept {
    return DVector3{static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(z)};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief Return one-dimensional data chunk of DVectorInt3.
  ///
  std::array<int32_t, 3> Data() const noexcept {
    return {x, y, z};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Float type length of this DVectorInt3.
  ///
  float GetLength() const noexcept {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Float squared length of this DVectorInt3.
  ///
  float GetSquareLength() const noexcept {
    return static_cast<float>(x * x + y * y + z * z);
  }

  //!
  //! Operators
  //!

  friend DVectorInt3 operator+(DVectorInt3 lhs, const DVectorInt3& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
  }

  friend DVectorInt3 operator-(DVectorInt3 lhs, const DVectorInt3& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
  }

  ///
  /// DVectorInt3 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVectorInt3 operator*(DVectorInt3 lhs, const int32_t rhs) noexcept {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVectorInt3, element multiplication happens.
  ///
  friend DVectorInt3 operator*(DVectorInt3 lhs, const DVectorInt3& rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVectorInt3 operator/(DVectorInt3 lhs, const int32_t rhs) noexcept {
    if (rhs == 0) {
      PUSH_LOG_CRITICAL_EXT("DVectorInt3 could not be divided by {0}.", rhs);
    }
    else {
      lhs.x = static_cast<int>(static_cast<float>(lhs.x) / rhs);
      lhs.y = static_cast<int>(static_cast<float>(lhs.y) / rhs);
      lhs.z = static_cast<int>(static_cast<float>(lhs.z) / rhs);
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVectorInt3 operator/(DVectorInt3 lhs, const DVectorInt3& rhs) noexcept {
    if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0) {
      PUSH_LOG_CRITICAL_EXT(
          "DVectorInt2 could not be devided by 0 included DVectorInt3, ({0}, {1}, {2})",
          rhs.x, rhs.y);
    }
    else {
      lhs.x = static_cast<int>(static_cast<float>(lhs.x) / rhs.x);
      lhs.y = static_cast<int>(static_cast<float>(lhs.y) / rhs.y);
      lhs.z = static_cast<int>(static_cast<float>(lhs.z) / rhs.z);
    }

    return lhs;
  }

  DVectorInt3& operator+=(const DVectorInt3& value) noexcept {
    this->x += value.x;
    this->y += value.y;
    this->z += value.z;

    return *this;
  }

  DVectorInt3& operator-=(const DVectorInt3& value) noexcept {
    this->x -= value.x;
    this->y -= value.y;
    this->z -= value.z;

    return *this;
  }

  DVectorInt3& operator*=(const int32_t value) noexcept {
    this->x *= value;
    this->y *= value;
    this->z *= value;

    return *this;
  }

  DVectorInt3& operator*=(const DVectorInt3& value) noexcept {
    this->x *= value.x;
    this->y *= value.y;
    this->z *= value.z;

    return *this;
  }

  ///
  /// If lhs and rhs are DVectorInt3, element multiplication happens.
  ///
  DVectorInt3& operator/=(const int32_t value) noexcept {
    if (value == 0) {
      PUSH_LOG_CRITICAL_EXT("DVectorInt3 could not be divided by {0}.", value);
    }
    else {
      this->x = static_cast<int>(static_cast<float>(this->x) / value);
      this->y = static_cast<int>(static_cast<float>(this->y) / value);
      this->z = static_cast<int>(static_cast<float>(this->z) / value);
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVectorInt3& operator/=(const DVectorInt3& value) noexcept {
    if (value.x == 0 || value.y == 0 || value.z == 0) {
      PUSH_LOG_CRITICAL_EXT(
          "DVectorInt2 could not be devided by 0 included DVectorInt2, ({0}, {1}, {2})",
          value.x, value.y, value.z);
    }
    else {
      this->x = static_cast<int>(static_cast<float>(this->x) / value.x);
      this->y = static_cast<int>(static_cast<float>(this->y) / value.y);
      this->z = static_cast<int>(static_cast<float>(this->z) / value.z);
    }

    return *this;
  }

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return lhs.GetSquareLength() == rhs.GetSquareLength();
  }

private:
  friend bool operator<(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return rhs < lhs;
  }

  friend bool operator<=(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return !(lhs < rhs);
  }

  //!
  //! Static functions
  //!
public:

  ///
  /// @brief Do dot product of (x, y, z) R^3 vector.
  /// @return Dot product int32_t value.
  ///
  static int32_t Dot(const DVectorInt3& lhs, const DVectorInt3& rhs) noexcept {
    return lhs.x * rhs.x + lhs.y * rhs.y * lhs.z + rhs.z;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVectorInt3 CompMaxLength(const DVectorInt3& lhs,
                                   const DVectorInt3& rhs) noexcept {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVectorInt3 CompMinLength(const DVectorInt3& lhs,
                                   const DVectorInt3& rhs) noexcept {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief Check if vector DVectorInt3 is all zero or nearly equal to zero.
  ///
  static bool IsAllZero(const opgs16::DVectorInt3& vector) noexcept;

  ///
  /// @brief Check if this DVectorInt3 is all zero or nearly equal to zero.
  ///
  bool IsAllZero() const noexcept;
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_VECTORI3_H
