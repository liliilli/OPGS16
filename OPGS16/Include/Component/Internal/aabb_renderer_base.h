#ifndef OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H
#define OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/aabb_renderer_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file.
///

#include <Component/Internal/renderer_base.h>
#include <Helper/Type/color.h>
#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CProtoRigidbodyCollider2D;
}

//!
//! Implementation
//!

namespace opgs16::component::_internal {

enum class EAabbColliderDmStyle {
  None,
  D2,
  D3,
};

class CPrivateAabbRendererBase : public CRendererBase {
public:
  CPrivateAabbRendererBase(
      element::CObject& bind_object,
      component::CProtoRigidbodyCollider2D* parent) :
      CRendererBase(bind_object), m_parent { parent }
  {};

  bool SetCollisionSize(const DVector3& collider_size);
  bool SetCollisionRenderPosition(const DVector3& final_position);

  const DVector3& GetCollisionSize() const noexcept;
  const DVector3& GetCollisionRenderPosition() const noexcept;
  EAabbColliderDmStyle GetColliderType() const noexcept;

  void Update(float delta_time) override final;
  virtual void Render() = 0;

protected:
  const glm::mat4& pGetModelMatrix() noexcept;
  void pSetAabbRenderingColor();

  CProtoRigidbodyCollider2D* m_parent = nullptr;

  mutable EAabbColliderDmStyle m_type = EAabbColliderDmStyle::None;
  opgs16::DColor m_state_color  = opgs16::DColor::Black;

private:
  void pUpdateModelMatrix();

  opgs16::DVector3 m_collider_size;
  opgs16::DVector3 m_render_position;
  glm::mat4 m_model_matrix = glm::mat4{};

  bool m_is_model_matrix_dirty = true;
  bool m_is_render_position_dirty = true;
  bool m_is_collider_size_dirty = true;
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H