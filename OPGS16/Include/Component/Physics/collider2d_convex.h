#ifndef OPGS16_COMPONENT_PHYSICS_COLLIDER2D_CONVEX_H
#define OPGS16_COMPONENT_PHYSICS_COLLIDER2D_CONVEX_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/collider2d_convex.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-02 Create file.
///

#include <Component/Internal/collider2d_base.h>

//!
//! Forward declaration
//!

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

class CColliderConvex2D final : public _internal::CCollider2DBase {
  using EColliderBehaviorState  = element::_internal::EColliderBehaviorState;
  using EColliderCollisionState = element::_internal::EColliderCollisionState;

public:
  CColliderConvex2D(element::CObject& bind_object) :
      CCollider2DBase{bind_object} {};

private:
  void Update(float delta_time) override final;

  ///
  /// @brief Initialize collider.
  /// This function should be called from bound rigidbody.
  ///
  void pInitializeCollider() override final;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CCollider2DBase, CColliderConvex2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_COLLIDER2D_CONVEX_H