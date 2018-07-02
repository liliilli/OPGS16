#ifndef OPGS16_HELPER_TYPE_VECTOR3_H
#define OPGS16_HELPER_TYPE_VECTOR3_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/vector3.h
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

#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <LinearMath/btVector3.h>

#include <Headers/import_logger.h>

#include <Helper/float.h>
#include <Helper/Type/vector2.h>

namespace opgs16 {

///
/// @struct DVector3
/// @brief Float type 3-element vector struct.
///
struct DVector3 {
  float x = 0.f;
  float y = 0.f;
  float z = 0.f;

  DVector3() = default;
  ~DVector3() = default;

  explicit DVector3(const float value) noexcept :
      x{value}, y{value}, z{value} {};

  explicit DVector3(const DVector2& value) noexcept :
      x{value.x}, y{value.y}, z{0.0f} {};

  DVector3(const float x, const float y, const float z) noexcept :
      x(x), y(y), z(z) {};

  DVector3(const DVector3&) = default;
  DVector3& operator=(const DVector3&) = default;

  DVector3(DVector3&&) = default;
  DVector3& operator=(DVector3&&) = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  explicit DVector3(const aiVector3D& value) noexcept :
      x{value.x}, y{value.y}, z{value.z} {}

  DVector3& operator=(const aiVector3D& value) noexcept {
    x = value.x;
    y = value.y;
    z = value.z;
    return *this;
  }

  explicit DVector3(const glm::vec3& value) noexcept :
      x{value.x}, y{value.y}, z{value.z} {};

  DVector3& operator=(const glm::vec3& value) noexcept {
    x = value.x;
    y = value.y;
    z = value.z;
    return *this;
  }

  explicit DVector3(const btVector3& value) noexcept :
      x{value.getX()}, y{value.getY()}, z{value.getZ()} {};

  DVector3& operator=(const btVector3& value) noexcept {
    x = value.getX();
    y = value.getY();
    z = value.getZ();
    return *this;
  }

  //!
  //! Conversion operators for dependencies.
  //!

  operator btVector3() const noexcept {
    return btVector3(x, y, z);
  }

  explicit operator glm::vec3() const noexcept {
    return {x, y, z};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief
  /// Return sequence data of this instance.
  ///
  std::array<float, 3> Data() const noexcept {
    return {x, y, z};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DVector3.
  ///
  float GetLength() const noexcept {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVector3.
  ///
  float GetSquareLength() const noexcept {
    return x * x + y * y + z * z;
  }

  ///
  /// @brief Return new DVector3 instance of normalized input vector.
  /// @return Normalized DVector3 vector.
  ///
  DVector3 Normalize() const noexcept {
    const auto length = this->GetLength();
    return {x / length, y / length, z / length};
  }

  //!
  //! Operators
  //!

  friend DVector3 operator+(DVector3 lhs, const DVector3& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
  }

  friend DVector3 operator-(DVector3 lhs, const DVector3& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
  }

  ///
  /// DVector3 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVector3 operator*(DVector3 lhs, const float rhs) noexcept {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVector3, element multiplication happens.
  ///
  friend DVector3 operator*(DVector3 lhs, const DVector3& rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVector3 operator/(DVector3 lhs, const float rhs) noexcept {
    if (rhs == 0.0f) {
      PUSH_LOG_CRITICAL_EXT("DVector3 could not be divided by {0}.", rhs);
    }
    else {
      lhs.x /= rhs;
      lhs.y /= rhs;
      lhs.z /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVector3 operator/(DVector3 lhs, const DVector3& rhs) noexcept {
    if (rhs.x == 0.0f || rhs.y == 0.0f || rhs.z == 0.0f) {
      PUSH_LOG_CRITICAL_EXT(
          "DVector3 could not be devided by 0 included DVector3, ({0}, {1}, {2})",
          rhs.x, rhs.y, rhs.z);
    }
    else {
      lhs.x /= rhs.x;
      lhs.y /= rhs.y;
      lhs.z /= rhs.z;
    }

    return lhs;
  }

  DVector3& operator+=(const DVector3& value) noexcept {
    this->x += value.x;
    this->y += value.y;
    this->z += value.z;
    return *this;
  }

  DVector3& operator-=(const DVector3& value) noexcept {
    this->x -= value.x;
    this->y -= value.y;
    this->z -= value.z;
    return *this;
  }

  DVector3& operator*=(const float value) noexcept {
    this->x *= value;
    this->y *= value;
    this->z *= value;
    return *this;
  }

  DVector3& operator*=(const DVector3& value) noexcept {
    this->x *= value.x;
    this->y *= value.y;
    this->z *= value.z;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector3, element multiplication happens.
  ///
  DVector3& operator/=(const float value) noexcept {
    if (value == 0.0f) {
      PUSH_LOG_CRITICAL_EXT("DVector3 could not be divided by {0}.", value);
    }
    else {
      this->x /= value;
      this->y /= value;
      this->z /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVector3& operator/=(const DVector3& value) noexcept {
    if (value.x == 0.0f || value.y == 0.0f || value.z == 0.0f) {
      PUSH_LOG_CRITICAL_EXT(
          "DVector3 could not be devided by 0 included DVector3, ({0}, {1}, {2})",
          value.x, value.y, value.z);
    }
    else {
      this->x /= value.x;
      this->y /= value.y;
      this->z /= value.z;
    }

    return *this;
  }

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @return Equal flag.
  ///
  friend bool operator==(const DVector3& lhs, const DVector3& rhs) noexcept {
    return __ApproximateEqual(lhs.GetSquareLength(), rhs.GetSquareLength(), 0.01f);
  }

private:
  friend bool operator<(const DVector3& lhs, const DVector3& rhs) noexcept {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVector3& lhs, const DVector3& rhs) noexcept {
    return rhs < lhs;
  }

  friend bool operator<=(const DVector3& lhs, const DVector3& rhs) noexcept {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVector3& lhs, const DVector3& rhs) noexcept {
    return !(lhs < rhs);
  }

  //!
  //! Static functions
  //!
public:

  ///
  /// @brief Do dot product of (x, y, z) R^3 vector.
  /// @return Dot product float value.
  ///
  static float Dot(const DVector3& lhs, const DVector3& rhs) noexcept {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  ///
  /// @brief Cross product of (x, y, z) R^3 vector.
  /// @return Cross product float value.
  ///
  static DVector3 Cross(const DVector3& lhs, const DVector3& rhs) noexcept {
    return {
        lhs.y * rhs.z - rhs.y * lhs.z,
        lhs.z * rhs.x - rhs.z * lhs.x,
        lhs.x * rhs.y - rhs.x * lhs.y
    };
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  ///
  static DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float value) noexcept {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector3 CompMaxLength(const DVector3& lhs, const DVector3& rhs) noexcept {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector3 CompMinLength(const DVector3& lhs, const DVector3& rhs) noexcept {
    return (lhs < rhs) ? lhs : rhs;
  }
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_VECTOR3_H