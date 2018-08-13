#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Impl/object_impl.cc
///
/// @brief Implementation file of object_impl.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-05 Add rendering layer member functions.
/// 2018-03-11 Moved implementation contents into ::opgs16::element::_internal.
/// 2018-04-18 Change function and mechanism of rotation.
///

/// Header file
#include <Element/Impl/object_impl.h>

#include <glm/gtc/matrix_transform.hpp>

#include <Helper/Math/math.h>
#include <Manager/setting_manager.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.inl>

namespace {
constexpr float k_2pi{ 2 * glm::pi<float>() };

const static glm::vec3 k_vec3_1{ 1.f };
const static glm::mat4 k_rotation_init{ glm::rotate(glm::mat4{}, k_2pi, k_vec3_1) };

bool IsAllAngleValueZero(const opgs16::DVector3& angle_array) {
  using opgs16::math::IsNearlyEqual;

  unsigned zero_count{ 0 };
  if (IsNearlyEqual(angle_array.x, 0.f, 0.001f)) zero_count++;
  if (IsNearlyEqual(angle_array.y, 0.f, 0.001f)) zero_count++;
  if (IsNearlyEqual(angle_array.z, 0.f, 0.001f)) zero_count++;

  return zero_count >= 3;
}

glm::mat4 GetRotationMatrix(const opgs16::DVector3& angle) {
  return glm::orientate4(glm::vec3{ glm::radians(angle.x), glm::radians(angle.z), glm::radians(angle.y) });
}

opgs16::DVector3 GetRotatedVector(const glm::vec3& vector, const opgs16::DVector3& angle) {
  using glm::rotateX;
  using glm::rotateY;
  using glm::rotateZ;

  return
  rotateZ(
    rotateY(
      rotateX(glm::vec3{1, 0, 0}, glm::radians(angle.x)),
      glm::radians(angle.y)),
    glm::radians(angle.z)
  );
}

} /// unnamed namespace

//!
//! Implementation
//!

namespace opgs16::element::_internal {

void CObjectImpl::pUpdateObjectSpaceAxisBasis() const noexcept {
  m_object_space_axis[0] = GetRotatedVector(glm::vec3{1, 0, 0}, m_propagated_world_rotation_euler_angle);
  m_object_space_axis[1] = GetRotatedVector(glm::vec3{0, 1, 0}, m_propagated_world_rotation_euler_angle);
  m_object_space_axis[2] = GetRotatedVector(glm::vec3{0, 0, 1}, m_propagated_world_rotation_euler_angle);

  m_is_world_space_axis_dirty = false;
}

void CObjectImpl::pUpdatePropagationAxisBasis() noexcept {
  m_object_propagate_axis[0] = GetRotatedVector(glm::vec3{1, 0, 0}, m_summed_world_rotation_euler_angle);
  m_object_propagate_axis[1] = GetRotatedVector(glm::vec3{0, 1, 0}, m_summed_world_rotation_euler_angle);
  m_object_propagate_axis[2] = GetRotatedVector(glm::vec3{0, 0, 1}, m_summed_world_rotation_euler_angle);

  m_is_world_propagation_axis_dirty = false;
}

void CObjectImpl::pUpdateSummedWorldRotationEulerAngle() noexcept {
  m_summed_world_rotation_euler_angle = m_propagated_world_rotation_euler_angle + m_world_rotation_euler_angle;
  m_summed_world_rotation_euler_angle.x = math::GetRotationAngle(m_summed_world_rotation_euler_angle.x);
  m_summed_world_rotation_euler_angle.y = math::GetRotationAngle(m_summed_world_rotation_euler_angle.y);
  m_summed_world_rotation_euler_angle.z = math::GetRotationAngle(m_summed_world_rotation_euler_angle.z);

  m_is_summed_rotation_angle_dirty  = false;
  m_is_final_rotation_angle_dirty   = true;
}

void CObjectImpl::pUpdateAxisAlignedLocalPosition() const noexcept {
  if (m_is_world_space_axis_dirty) pUpdateObjectSpaceAxisBasis();

  m_local_axis_arranged_position = DVector3{};
  m_local_axis_arranged_position += m_object_space_axis[0] * m_independent_local_position.x;
  m_local_axis_arranged_position += m_object_space_axis[1] * m_independent_local_position.y;
  m_local_axis_arranged_position += m_object_space_axis[2] * m_independent_local_position.z;

  m_is_local_position_dirty = false;
}

void CObjectImpl::pUpdateAxisAlignedWorldPosition() const noexcept {
  if (m_is_world_space_axis_dirty) pUpdateObjectSpaceAxisBasis();

  m_world_axis_arranged_position = DVector3{};
  m_world_axis_arranged_position += m_object_space_axis[0] * m_independent_world_position.x;
  m_world_axis_arranged_position += m_object_space_axis[1] * m_independent_world_position.y;
  m_world_axis_arranged_position += m_object_space_axis[2] * m_independent_world_position.z;

  m_is_local_position_dirty = false;
}

void CObjectImpl::pUpdateAxisAlignedSummedWorldPosition() const noexcept {
  m_summed_world_position = m_propagated_world_basis_position + m_world_axis_arranged_position;
  m_is_summed_position_dirty = false;
}

void CObjectImpl::pUpdateFinalWorldRotationEulerAngle() const noexcept {
  m_object_final_rotation_euler_angle = m_summed_world_rotation_euler_angle + m_local_rotation_euler_angle;

  m_is_final_rotation_angle_dirty = false;
}

void CObjectImpl::pUpdateAxisAlignedFinalPosition() const {
  if (m_is_model_matrix_dirty) pUpdateObjectSpaceAxisBasis();
  if (m_is_local_position_dirty) pUpdateAxisAlignedLocalPosition();
  if (m_is_world_position_dirty) pUpdateAxisAlignedWorldPosition();
  if (m_is_summed_position_dirty) pUpdateAxisAlignedSummedWorldPosition();

  m_axis_aligned_final_position = m_summed_world_position + m_local_axis_arranged_position;
#ifdef false
  const auto object_space_position = m_independent_local_position + m_independent_world_position;

  m_axis_aligned_final_position += m_object_space_axis[0] * object_space_position.x;
  m_axis_aligned_final_position += m_object_space_axis[1] * object_space_position.y;
  m_axis_aligned_final_position += m_object_space_axis[2] * object_space_position.z;
#endif

  m_is_final_position_dirty = false;
}

void CObjectImpl::RefreshRotationMatrix() const {
  if (IsAllAngleValueZero(m_object_final_rotation_euler_angle))
    m_rotation_matrix = k_rotation_init;
  else
    m_rotation_matrix = GetRotationMatrix(m_object_final_rotation_euler_angle);
}

void CObjectImpl::RefreshScaleVector() const {
  m_scale_final_vector = m_scale_local_factor;
}

const glm::mat4& CObjectImpl::GetModelMatrix() const {
	if (m_is_model_matrix_dirty) {
    if (m_is_final_rotation_angle_dirty) {
      pUpdateFinalWorldRotationEulerAngle();
      RefreshRotationMatrix();
    }

    if (m_is_final_position_dirty) {
      pUpdateAxisAlignedFinalPosition();
    }

    if (m_is_local_rotation_angle_dirty) {
      m_is_local_rotation_angle_dirty = false;
    }

    if (m_is_local_scale_dirty) {
      RefreshScaleVector();
      m_is_local_scale_dirty = false;
    }

    m_is_model_matrix_dirty = false;
  }

  m_final_model     = m_rotation_matrix;
  m_final_model[0] *= m_scale_final_vector.x;
  m_final_model[1] *= m_scale_final_vector.y;
  m_final_model[2] *= m_scale_final_vector.z;
  m_final_model[3][0] = m_axis_aligned_final_position.x;
  m_final_model[3][1] = m_axis_aligned_final_position.y;
  m_final_model[3][2] = m_axis_aligned_final_position.z;

  return m_final_model;
}

std::string CObjectImpl::GetTagNameOf() const {
  return manager::setting::GetTagName(m_tag_index);
}

void CObjectImpl::SetTag(const std::string& tag_name) {
    auto& tag_list = manager::setting::GetTagNameList();

    decltype(tag_list.size()) i = 0;
    for (; i < tag_list.size(); ++i) {
        if (tag_name == tag_list[i]) {
            m_tag_index = static_cast<int32_t>(i);
            break;
        }
    }

    if (i == tag_list.size()) m_tag_index = 0;
}

void CObjectImpl::SetTag(const unsigned tag_index) {
    const auto list_size = manager::setting::GetTagNameList().size();

    if (tag_index >= list_size)
        m_tag_index = 0;
    else
        m_tag_index = tag_index;
}

} /// ::opgs16::element::_internal namespace



