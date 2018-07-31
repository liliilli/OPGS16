#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/aabb_renderer_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file.
///

/// Header file
#include <Component/Internal/aabb_renderer_base.h>
#include <Manager/object_manager.h>

namespace {

glm::mat3 GetRotationMatrix(const opgs16::DVector3& rotation_angle) {
  float min = rotation_angle.x;
  float max = rotation_angle.x;
  if (rotation_angle.y < min) { min = rotation_angle.y; }
  if (rotation_angle.y > max) { max = rotation_angle.y; }
  if (rotation_angle.z < min) { min = rotation_angle.z; }
  if (rotation_angle.z > max) { max = rotation_angle.z; }

  if (std::abs(min) > max) {
    max = min;
  }

  return glm::rotate(
      glm::mat4{}, glm::radians(max),
      glm::vec3{ rotation_angle.x / max,
                 rotation_angle.y / max,
                 rotation_angle.z / max}
  );
}

} /// ::unnamed namespace

namespace opgs16::component::_internal {

bool CPrivateAabbRendererBase::SetCollisionSize(const DVector3& collider_size) {
  m_collider_size = collider_size;

  m_is_model_matrix_dirty     = true;
  m_is_collider_size_dirty    = true;
  return true;
}

bool CPrivateAabbRendererBase::SetCollisionRenderPosition(const DVector3& final_position) {
  m_render_position = final_position;

  m_is_model_matrix_dirty     = true;
  m_is_render_position_dirty  = true;
  return true;
}

const DVector3& CPrivateAabbRendererBase::GetCollisionSize() const noexcept {
  return m_collider_size;
}

const DVector3& CPrivateAabbRendererBase::GetCollisionRenderPosition() const noexcept {
  return m_render_position;
}

EAabbColliderStyle CPrivateAabbRendererBase::GetColliderType() const noexcept {
  return m_type;
}

const glm::mat4& CPrivateAabbRendererBase::PGetModelMatrix() noexcept {
  if (m_is_model_matrix_dirty) {
    pUpdateModelMatrix();
    m_is_model_matrix_dirty = false;
  }

  return m_model_matrix;
}

void CPrivateAabbRendererBase::pUpdateModelMatrix() {
  if (m_is_collider_size_dirty) {
    m_model_matrix[0][0] = m_collider_size.x / 2;
    m_model_matrix[1][1] = m_collider_size.y / 2;
    m_model_matrix[2][2] = m_collider_size.z / 2;
    m_is_collider_size_dirty = false;
  }

  if (m_is_render_position_dirty) {
    m_model_matrix[3][0] = m_render_position.x;
    m_model_matrix[3][1] = m_render_position.y;
    m_model_matrix[3][2] = m_render_position.z;
    m_is_render_position_dirty = false;
  }
}

void CPrivateAabbRendererBase::Update(float delta_time) {
  using opgs16::manager::object::InsertAABBInformation;

  if (m_is_model_matrix_dirty) {
    pUpdateModelMatrix();
    m_is_model_matrix_dirty = false;
  }

  InsertAABBInformation(*this);
}

} /// ::opgs16::component::_internal namespace