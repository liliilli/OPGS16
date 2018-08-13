#ifndef OPGS16_HELEPR_TYPE_QUATERNION_H
#define OPGS16_HELEPR_TYPE_QUATERNION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/quaternion.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file.
///

#include <glm/gtc/quaternion.hpp>

//!
//! Forward declaration
//!

namespace opgs16 {
enum class EAxis3D;
struct DVector3;
}

//!
//! Implementation
//!

namespace opgs16 {
  class DQuaternion final {
public:
  DQuaternion() = default;

  DQuaternion(const opgs16::DVector3& euler_angle);

  ///
  /// @brief Get rotation matrix (4x4) from quaternion.
  ///
  glm::mat4 GetRotationMatrix() const noexcept;

  ///
  /// @brief Return euler rotation angle (degrees) (x, y, z).
  ///
  DVector3 GetEulerRotationDegreeAngleVector() const noexcept;

  ///
  /// @brief Return euler rotation angle (radians) (x, y, z).
  ///
  DVector3 GetEulerRotationRadiansAngleVector() const noexcept;

  ///
  /// @brief Get rotation angle with radians.
  ///
  float GetRadiansAngle() const noexcept;

  ///
  /// @brief Impulse rotation with euler_angle vector (degrees).
  ///
  void AddRotationAngle(const opgs16::DVector3& euler_angle);

  ///
  /// @brief
  ///
  void AddRotationAngle(const DQuaternion& quaternion_angle);

  ///
  /// @brief
  ///
  void AddRotationAngle(opgs16::EAxis3D axis, float degree_angle);

  ///
  /// @brief
  ///
  void SetRotationAngle(const opgs16::DVector3& euler_angle);

private:
  const glm::quat& pGetQuaternion() const noexcept;

  glm::quat m_quaternion;
};

} /// ::opgs16 namespace

#endif /// OPGS16_HELEPR_TYPE_QUATERNION_H