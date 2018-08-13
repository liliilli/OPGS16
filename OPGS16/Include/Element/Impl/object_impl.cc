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
  return glm::orientate4(glm::vec3{ glm::radians(angle.x), glm::radians(angle.y), glm::radians(angle.z) });
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

void CObjectImpl::pUpdateObjectSpaceAxisBasis() noexcept {
  m_object_space_axis[0] = GetRotatedVector(glm::vec3{1, 0, 0}, m_propagated_world_rotation_euler_angle);
  m_object_space_axis[1] = GetRotatedVector(glm::vec3{0, 1, 0}, m_propagated_world_rotation_euler_angle);
  m_object_space_axis[2] = GetRotatedVector(glm::vec3{0, 0, 1}, m_propagated_world_rotation_euler_angle);
}

void CObjectImpl::pUpdatePropagationAxisBasis() noexcept {
  m_object_propagate_axis[0] = GetRotatedVector(glm::vec3{1, 0, 0}, m_summed_world_rotation_euler_angle);
  m_object_propagate_axis[1] = GetRotatedVector(glm::vec3{0, 1, 0}, m_summed_world_rotation_euler_angle);
  m_object_propagate_axis[2] = GetRotatedVector(glm::vec3{0, 0, 1}, m_summed_world_rotation_euler_angle);
}

void CObjectImpl::pUpdateSummedWorldRotationEulerAngle() noexcept {
  m_summed_world_rotation_euler_angle = m_propagated_world_rotation_euler_angle + m_world_rotation_euler_angle;
  m_summed_world_rotation_euler_angle.x = math::GetRotationAngle(m_summed_world_rotation_euler_angle.x);
  m_summed_world_rotation_euler_angle.y = math::GetRotationAngle(m_summed_world_rotation_euler_angle.y);
  m_summed_world_rotation_euler_angle.z = math::GetRotationAngle(m_summed_world_rotation_euler_angle.z);
}

  void CObjectImpl::RefreshFinalPosition() const {
  m_final_position = m_local_position + m_parent_from_position;

  m_final_position.x += m_wp_rotate_matrix[0][0] * m_world_position.x;
  m_final_position.x += m_wp_rotate_matrix[1][0] * m_world_position.y;
  m_final_position.x += m_wp_rotate_matrix[2][0] * m_world_position.z;

  m_final_position.y += m_wp_rotate_matrix[0][1] * m_world_position.x;
  m_final_position.y += m_wp_rotate_matrix[1][1] * m_world_position.y;
  m_final_position.y += m_wp_rotate_matrix[2][1] * m_world_position.z;

  m_final_position.z += m_wp_rotate_matrix[0][2] * m_world_position.x;
  m_final_position.z += m_wp_rotate_matrix[1][2] * m_world_position.y;
  m_final_position.z += m_wp_rotate_matrix[2][2] * m_world_position.z;
}

void CObjectImpl::RefreshRotateMatrix() const {
  if (IsAllAngleValueZero(m_local_rotation_euler_angle))
    m_local_rotate_matrix = k_rotation_init;
  else
    m_local_rotate_matrix = GetRotationMatrix(m_local_rotation_euler_angle);
}

void CObjectImpl::RefreshWpRotationMatrix() const {
  if (IsAllAngleValueZero(m_object_final_rotation_euler_angle))
    m_wp_rotate_matrix = k_rotation_init;
  else
    m_wp_rotate_matrix = GetRotationMatrix(m_object_final_rotation_euler_angle);
}

void CObjectImpl::RefreshScaleVector() const {
  m_scale_final_vector = m_scale_local_factor;
}

const glm::mat4& CObjectImpl::GetModelMatrix() const {
  if (m_offset_model_matrix_deprecated) {
    RefreshWpRotationMatrix();
    RefreshFinalPosition();
    m_offset_model_matrix_deprecated = false;
  }

	if (m_is_local_model_matrix_deprecated) {
    if (m_final_pos_deprecated) {
      RefreshFinalPosition();
      m_final_pos_deprecated = false;
    }

    if (m_local_rotation_deprecated) {
      RefreshRotateMatrix();
      m_local_rotation_deprecated = false;
    }

    if (m_is_local_scale_dirty) {
      RefreshScaleVector();
      m_is_local_scale_dirty = false;
    }

    m_is_local_model_matrix_deprecated = false;
  }

  m_final_model     = m_wp_rotate_matrix * m_local_rotate_matrix;
  m_final_model[0] *= m_scale_final_vector.x;
  m_final_model[1] *= m_scale_final_vector.y;
  m_final_model[2] *= m_scale_final_vector.z;
  m_final_model[3][0] = m_final_position.x;
  m_final_model[3][1] = m_final_position.y;
  m_final_model[3][2] = m_final_position.z;

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

