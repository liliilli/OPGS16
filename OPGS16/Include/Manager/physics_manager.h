#ifndef OPGS16_MANAGER_PHYSICS_MANAGER_H
#define OPGS16_MANAGER_PHYSICS_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/physics_manager.h
/// @brief
/// The file which contains management component that manages physics objects.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-15 Create file. Implement fundamental mechanism.
/// 2018-03-04 Refactoring.
/// 2018-05-31 Refactoring. Detach singleton to namepsace following that.
///

/// Forward declaration
#include <opgs16fwd.h>

//!
//! Forward declaration
//!

namespace opgs16::manager::physics::_internal {
class CPhysicsEnvironment;
class CCollisionShapeList;
}

//!
//! Implementation
//!

///
/// @namespace opgs16::manager::physics
/// @brief Physics manager manages object collision and rigidbody movement.
///
namespace opgs16::manager::physics {

///
/// @brief
/// Add AABB collider to be performed collision check by physics manager.
///
/// @param[in] collider 2D Collider to apply.
/// @param[in] rigidbody 2D rigidbody to apply.
///
void AddCollider(opgs16::physics::CRectangleCollider2D* const collider,
                 component::CRigidbody2D* const rigidbody);

///
/// @brief
/// Update physics manager namespace to calculate physics collision process.
///
void Update();

///
/// @brief
/// Render collision box when _OPGS16_DEBUG_OPTION is defined at manifest.h.
///
void RenderCollisionBox();

///
/// @brief
/// Release and clean physics object container.
///
void Clear();

///
/// @brief
/// Return reference of physics environment managerial instance.
///
/// @return Reference of physics environment instance.
///
_internal::CPhysicsEnvironment& GetManagement();

///
/// @brief
/// Return reference of collision shape list.
///
/// @return Reference of collision shape list.
///
_internal::CCollisionShapeList& GetShapeList();

} /// ::opgs16::manager::physics namespace.

#endif /// OPGS16_MANAGER_PHYSICS_MANAGER_H
