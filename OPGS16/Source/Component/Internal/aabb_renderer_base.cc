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

#include <Component/Physics/prot_rigidbody_collider2d.h>
#include <Manager/object_manager.h>

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

EAabbColliderDmStyle CPrivateAabbRendererBase::GetColliderType() const noexcept {
  return m_type;
}

const glm::mat4& CPrivateAabbRendererBase::pGetModelMatrix() noexcept {
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

void CPrivateAabbRendererBase::pSetAabbRenderingColor() {
  using opgs16::element::_internal::EColliderBehaviorState;
  using opgs16::element::_internal::EColliderActualType;
  if (!m_parent) PHITOS_UNEXPECTED_BRANCH();

  const auto state = m_parent->GetColliderState();
  PHITOS_ASSERT(state != EColliderBehaviorState::None,
                "Collision state must not be None.");

  switch (state) {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case EColliderBehaviorState::Activated: {
    switch (m_parent->GetColliderActualType()) {
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    case EColliderActualType::Kinetic: m_state_color = DColor::Aqua; break;
    case EColliderActualType::Dynamic: m_state_color = DColor::Green; break;
    case EColliderActualType::Staic:   m_state_color = DColor::Orange; break;
    }
  } break;
  case EColliderBehaviorState::Collided: m_state_color = DColor::Yellow; break;
  case EColliderBehaviorState::Sleep:    m_state_color = DColor::Gray; break;
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