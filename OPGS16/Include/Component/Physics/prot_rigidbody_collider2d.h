#ifndef OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H
#define OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/prot_rigidbody_collider2d.h
///
/// @author Jongmin Yun
///
/// @log
/// Create file.
///

#include <string>

#include <Component/Internal/collider2d_base.h>
#include <Element/Internal/physics_collider_bind_info.h>
#include <Element/Internal/physics_enums.h>
#include <Helper/Type/vector2.h>
#include <Component/Internal/aabb_renderer_2d.h>

//!
//! Forward declaration
//!

namespace opgs16 {
struct DVector3;
}
namespace opgs16::manager::physics::_internal {
class CPhysicsEnvironment;
}
namespace opgs16::component::_internal {
class CPrivateAabbRenderer2D;
}

//!
//! Implementation
//!

namespace opgs16::component {

class CColliderBox2D final : public _internal::CCollider2DBase {
  using EColliderBehaviorState = element::_internal::EColliderBehaviorState;
  using EColliderCollisionState = element::_internal::EColliderCollisionState;
  using TAabbRendererSmtPtr = std::unique_ptr<_internal::CPrivateAabbRenderer2D>;

public:
  CColliderBox2D(element::CObject& bind_object, const DVector2& collider_size);

  void TemporalSetStatic();

  ///
  /// @brief Set trigger or collision flag.
  ///
  void SetTriggered(bool is_triggered);

  bool IsTriggered() const noexcept;

  ///
  /// @update
  /// Frequent call will cause performance decrease.
  ///
  void SetColliderSize(const DVector2& size);

  EColliderBehaviorState GetColliderState() const noexcept;

private:
  void Update(float delta_time) override final;

  ///
  /// @brief
  ///
  void pInitializeCollider() override final;

  ///
  /// @brief
  ///
  void pCreatebtRigidbody(const float mass_sum, btRigidBody** rigidbody_ptr);

  ///
  /// @brief
  ///
  void pUpdateAabbToRenderer(const DVector3& min, const DVector3& max);

  ///
  /// @brief
  ///
  void pSetCollisionState(EColliderBehaviorState state);

  ///
  /// @brief
  ///
  void pCallBindObjectCallback(CColliderBox2D* other_collider);

  ///
  /// @brief
  ///
  bool pIsCallbackFunctionCalledOnThisFrame() const noexcept;

  TAabbRendererSmtPtr m_aabb_renderer = nullptr;

  element::_internal::DPrivateColliderBindInfo m_bind_info;
  EColliderBehaviorState  m_state = EColliderBehaviorState::None;
  EColliderCollisionState m_collision_state = EColliderCollisionState::Idle;

  opgs16::DVector2 m_collider_size;

  bool m_is_position_initialized = false;
  bool m_is_collided_flag_setup  = false;
  bool m_is_collision_triggered  = false;

  friend opgs16::manager::physics::_internal::CPhysicsEnvironment;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CCollider2DBase, CColliderBox2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H