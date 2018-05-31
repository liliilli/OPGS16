#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/physics_internal.h
/// @brief
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Create file. Move PhysicsManager::Item to this file.
///

/// ::opgs16::component::CRigidbody2D
#include <Component/rigidbody_2d.h>

namespace opgs16::manager::_internal {

struct Item {
  opgs16::physics::CRectangleCollider2D* const m_collider;
  component::CRigidbody2D* const m_rigidbody;
  const glm::vec2 m_position;

  enum class Type {
    Begin,
    End
  };
  Type m_type = { Type::Begin };

  explicit Item(opgs16::physics::CRectangleCollider2D* const collider,
                component::CRigidbody2D* const rigidbody,
                const glm::vec2 axis_value,
                Type type) :
    m_collider{ collider }, m_rigidbody{ rigidbody },
    m_position{ axis_value }, m_type{ type } {};
};

} /*! opgs16::manager::_internal */

#endif // !OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H

