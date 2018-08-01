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

public:
  CColliderBox2D(element::CObject& bind_object, const DVector2& collider_size);

  ///
  /// @update
  /// Frequent call will cause performance decrease.
  ///
  void SetColliderSize(const DVector2& size);

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

  element::_internal::DPrivateColliderBindInfo m_bind_info;

  opgs16::DVector2 m_collider_size;

  bool m_is_position_initialized = false;
  bool m_is_collided_flag_setup  = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CCollider2DBase, CColliderBox2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H