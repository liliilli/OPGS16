#ifndef OPGS16_HELPER_TYPE_VECTOR2_H
#define OPGS16_HELPER_TYPE_VECTOR2_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/vector2.h
///
/// @brief
/// Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-30 Create file.
///

#include <array>

#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <LinearMath/btVector3.h>

#include <Headers/import_logger.h>
#include <Helper/float.h>

namespace opgs16 {

///
/// @struct DVector2
/// @brief Float type 2-element vector struct.
///
struct DVector2 {
  float x = 0.f;
  float y = 0.f;

  DVector2() = default;
  ~DVector2() = default;

  DVector2(const float x, const float y) noexcept :
      x{x}, y{y} {};

  explicit DVector2(const float value) noexcept :
      x{value}, y{value} { }

  DVector2(const DVector2&) = default;
  DVector2& operator=(const DVector2& value) = default;

  DVector2(DVector2&&) = default;
  DVector2& operator=(DVector2&&) = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  explicit DVector2(const aiVector2D& value) noexcept :
      x{value.x}, y{value.y} {};

  DVector2& operator=(const aiVector2D& value) noexcept {
    x = value.x;
    y = value.y;
    return *this;
  }

  explicit DVector2(const aiVector3D& value) noexcept :
      x{value.x}, y{value.y} {};

  DVector2& operator=(const aiVector3D& value) noexcept {
    x = value.x;
    y = value.y;
    return *this;
  }

  explicit DVector2(const glm::vec2& value) noexcept :
      x{value.x}, y{value.y} {};

  DVector2& operator=(const glm::vec2& value) noexcept {
    x = value.x;
    y = value.y;
    return *this;
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator btVector3() const noexcept {
    return btVector3{x, y, 0.f};
  }

  operator glm::vec2() const noexcept {
    return glm::vec2{x, y};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief Return one-dimensional data chunk of DVector2.
  ///
  std::array<float, 2> Data() const noexcept {
    return {x, y};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DVector2.
  ///
  float GetLength() const noexcept {
    return std::sqrtf(x * x + y * y);
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVector2.
  ///
  float GetSquareLength() const noexcept {
    return x * x + y * y;
  }

  ///
  /// @brief Return new DVector2 instance of normalized input vector.
  /// @return Normalized DVector2 vector.
  ///
  DVector2 Normalize() const noexcept {
    const auto length = this->GetLength();
    return {x / length, y / length};
  }

  //!
  //! Operators
  //!

  friend DVector2 operator+(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
  }

  friend DVector2 operator-(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
  }

  ///
  /// DVector2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVector2 operator*(DVector2 lhs, const float rhs) noexcept {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVector2, element multiplication happens.
  ///
  friend DVector2 operator*(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVector2 operator/(DVector2 lhs, const float rhs) noexcept {
    if (rhs == 0.0f) {
      PUSH_LOG_CRITICAL_EXT("DVector2 could not be divided by {0}.", rhs);
    }
    else {
      lhs.x /= rhs;
      lhs.y /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVector2 operator/(DVector2 lhs, const DVector2& rhs) noexcept {
    if (rhs.x == 0.0f || rhs.y == 0.0f) {
      PUSH_LOG_CRITICAL_EXT(
          "DVector2 could not be devided by 0 included DVector2, ({0}, {1})",
          rhs.x, rhs.y);
    }
    else {
      lhs.x /= rhs.x;
      lhs.y /= rhs.y;
    }

    return lhs;
  }

  DVector2& operator+=(const DVector2& value) noexcept {
    this->x += value.x;
    this->y += value.y;
    return *this;
  }

  DVector2& operator-=(const DVector2& value) noexcept {
    this->x -= value.x;
    this->y -= value.y;
    return *this;
  }

  DVector2& operator*=(const float value) noexcept {
    this->x *= value;
    this->y *= value;
    return *this;
  }

  DVector2& operator*=(const DVector2& value) noexcept {
    this->x *= value.x;
    this->y *= value.y;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector2, element multiplication happens.
  ///
  DVector2& operator/=(const float value) noexcept {
    if (value == 0.0f) {
      PUSH_LOG_CRITICAL_EXT("DVector2 could not be divided by {0}.", value);
    }
    else {
      this->x /= value;
      this->y /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVector2& operator/=(const DVector2& value) noexcept {
    if (value.x == 0.0f || value.y == 0.0f) {
      PUSH_LOG_CRITICAL_EXT(
          "DVector2 could not be devided by 0 included DVector2, ({0}, {1})",
          value.x, value.y);
    }
    else {
      this->x /= value.x;
      this->y /= value.y;
    }

    return *this;
  }

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DVector2& lhs, const DVector2& rhs) noexcept {
    return __ApproximateEqual(lhs.GetSquareLength(), rhs.GetSquareLength(), 0.01f);
  }

private:
  friend bool operator<(const DVector2& lhs, const DVector2& rhs) noexcept {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVector2& lhs, const DVector2& rhs) noexcept {
    return rhs < lhs;
  }

  friend bool operator<=(const DVector2& lhs, const DVector2& rhs) noexcept {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVector2& lhs, const DVector2& rhs) noexcept {
    return !(lhs < rhs);
  }

public:

  ///
  /// @brief Check if this DVector2 is all zero or nearly equal to zero.
  ///
  bool IsAllZero() const noexcept;

  //!
  //! Static functions
  //!
public:

  ///
  /// @brief Do dot product of (x, y) R^2 vector.
  /// @return Dot product float value.
  ///
  static float Dot(const DVector2& lhs, const DVector2& rhs) noexcept {
    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  ///
  static DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float value) noexcept {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector2 CompMaxLength(const DVector2& lhs, const DVector2& rhs) noexcept {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector2 CompMinLength(const DVector2& lhs, const DVector2& rhs) noexcept {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] normalized
  /// @return
  ///
  static float AngleRad(const DVector2& lhs, const DVector2& rhs,
                        bool normalized = true) noexcept {
    auto nlhs = lhs;
    auto nrhs = rhs;
    if (!normalized) {
      nlhs = lhs.Normalize();
      nrhs = rhs.Normalize();
    }

    const auto cos = nrhs.x * nlhs.x + nrhs.y * nlhs.y;
    const auto sin = nrhs.y * nlhs.x - nrhs.x * nlhs.y;
    const auto degree = std::acosf(cos);

    return (sin < 0.0f) ? -degree : degree;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] normalized
  /// @return
  ///
  static float AngleDeg(const DVector2& lhs, const DVector2& rhs,
                        bool normalized = true) noexcept {
    return AngleRad(lhs, rhs, normalized) * 180.f / 3.1415926535f;
  }

  ///
  /// @brief Check if vector DVector2 is all zero or nearly equal to zero.
  ///
  static bool IsAllZero(const opgs16::DVector2& vector) noexcept;
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_VECTOR2_H