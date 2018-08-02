#ifndef OPGS16_ELEMENT_INTERNAL_PHYSICS_ENUMS_H
#define OPGS16_ELEMENT_INTERNAL_PHYSICS_ENUMS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/physics_enums.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-31 Create file.
///

namespace opgs16::element::_internal {

enum class EColliderBehaviorState {
  /// Activated (Aqua, Green or Orange)
  Activated,
  /// Collided with any collider. (Yellow)
  Collided,
  /// Is sleep (Light-Gray)
  Sleep,

  /// Not initiated
  None
};

enum class EColliderActualType {
  /// Both CCollider and CRigidbody activated but is kinetic. (Aqua)
  Kinetic,
  /// Both CCollider and CRigidbody activated and not kinetic, has mass. (Green)
  Dynamic,
  /// CCollider activated but CRigidbody (not btRigidbody!) deactivated. (Orange)
  Static,

  /// None. Not initiated.
  None
};

enum class EColliderCollisionState {
  Idle, /// Exit not needed.
  Enter,
  Stay,
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PHYSICS_ENUMS_H