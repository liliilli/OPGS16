#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/quaternion.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file.
///

/// Header file
#include <Helper/Type/quaternion.h>
#include <Helper/Math/math.h>
#include <Helper/Type/axis.h>

#include <glm/gtx/quaternion.hpp>

namespace opgs16 {

DQuaternion::DQuaternion(const opgs16::DVector3& euler_angle) :
    m_quaternion{ glm::vec3{math::DegToRad(euler_angle.x),
                            math::DegToRad(euler_angle.y),
                            math::DegToRad(euler_angle.z)} } {}

glm::mat4 DQuaternion::GetRotationMatrix() const noexcept {
  return glm::mat4_cast(m_quaternion);
}

DVector3 DQuaternion::GetEulerRotationDegreeAngleVector() const noexcept {
  const auto radians = glm::eulerAngles(m_quaternion);
  return {math::RadToDeg(radians.x), math::RadToDeg(radians.y), math::RadToDeg(radians.z)};
}

DVector3 DQuaternion::GetEulerRotationRadiansAngleVector() const noexcept {
  return glm::eulerAngles(m_quaternion);
}

float DQuaternion::GetRadiansAngle() const noexcept {
  return glm::angle(m_quaternion);
}

void DQuaternion::AddRotationAngle(const opgs16::DVector3& euler_angle) {
  const auto degree = GetEulerRotationDegreeAngleVector() + euler_angle;

  m_quaternion = glm::quat{
    glm::vec3(glm::radians(degree.x), glm::radians(degree.y), glm::radians(degree.z))
  };
}

void DQuaternion::AddRotationAngle(const DQuaternion& quaternion_angle) {
  m_quaternion = quaternion_angle.pGetQuaternion() * m_quaternion;

#ifdef false
  m_quaternion = rotate(
      m_quaternion,
      quaternion_angle.GetRadiansAngle(),
      static_cast<glm::vec3>(quaternion_angle.GetEulerRotationRadiansAngleVector())
  );
#endif
}

void DQuaternion::AddRotationAngle(opgs16::EAxis3D axis, float degree_angle) {
  const auto radians = degree_angle * math::deg_to_rad<float>;

  switch (axis) {
  case EAxis3D::X:
    m_quaternion = rotate(m_quaternion, radians, static_cast<glm::vec3>(DVector3::RightX()));
    break;
  case EAxis3D::Y:
    m_quaternion = rotate(m_quaternion, radians, static_cast<glm::vec3>(DVector3::UpY()));
    break;
  case EAxis3D::Z:
    m_quaternion = rotate(m_quaternion, radians, static_cast<glm::vec3>(DVector3::FrontZ()));
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

}

void DQuaternion::SetRotationAngle(const opgs16::DVector3& euler_angle) {
  m_quaternion = glm::quat{
      glm::vec3{math::DegToRad(euler_angle.x),
                math::DegToRad(euler_angle.y),
                math::DegToRad(euler_angle.z)}
  };
}

const glm::quat& DQuaternion::pGetQuaternion() const noexcept {
  return m_quaternion;
}

} /// ::opgs16 namespace