#ifndef OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLIDER_BIND_INFO_H
#define OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLIDER_BIND_INFO_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/physics_collider_bind_info.h
///
/// @brief Instructor struct class type for btRigidbody.
///
/// @author Jongmin Yun
///
/// @log
/// Create file.
///

//!
//! Forward declaration
//!

namespace opgs16::element {
class CObject;
}
namespace opgs16::component::_internal {
class CColliderBase;
}

//!
//! Implementation
//!

namespace opgs16::element::_internal {

struct DPrivateColliderBindInfo {
  opgs16::element::CObject* bind_object;
  opgs16::component::_internal::CColliderBase* bind_collider;
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLIDER_BIND_INFO_H