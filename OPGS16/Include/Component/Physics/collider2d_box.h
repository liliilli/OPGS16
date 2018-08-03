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

#include <Component/Internal/collider2d_base.h>
#include <Helper/Type/vector2.h>

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

///
/// @class CColliderBox2D
/// @brief Collision 2d box type which should be attached to any 2d rigidbody.
///
class CColliderBox2D final : public _internal::CCollider2DBase {
  using EColliderBehaviorState  = element::_internal::EColliderBehaviorState;
  using EColliderCollisionState = element::_internal::EColliderCollisionState;

public:
  CColliderBox2D(element::CObject& bind_object, const DVector2& collider_size);

  ///
  /// @update
  /// Frequent call after initiation will cause performance decrease.
  ///
  void SetColliderSize(const DVector2& size);

private:
  void Update(float delta_time) override final;

  ///
  /// @brief Initialize collider.
  /// This function should be called from bound rigidbody.
  ///
  void pInitializeCollider() override final;

  void pCreatebtRigidbody(float mass_sum, btRigidBody** rigidbody_ptr);

  DVector2 m_collider_size;
  bool m_is_position_initialized = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CCollider2DBase, CColliderBox2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H