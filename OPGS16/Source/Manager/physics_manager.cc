#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/physics_manager.cc
///
/// @brief
/// Implementation file of ::opgs16::manager::physics.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Refactoring.
/// 2018-05-31 Detach singleton to namespace following styleguide.
///

/// Header file
#include <Manager/physics_manager.h>

#include <cmath>

#include <algorithm>
#include <memory>
#include <vector>

/// phitos::debug Expanded assertion.
#include <Phitos/Dbg/assert.h>

/// opgs16::component::CRigidbody2D
#include <Component/Physics/rigidbody_2d.h>
/// collision::CRectangleCollider2D
#include <Component/Physics/rectangle.h>
/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::manager::MSettingManager
#include <Manager/setting_manager.h>
/// ::opgs16::manager::physics::_internal::CCollisionShapeList
#include <Manager/Physics/physics_collision_shape_list.h>
/// ::opgs16::manager::physics Strong enum flags.
#include <Manager/Physics/physics_flag.h>
/// ::opgs16::manager::_internal::Item
#include <Manager/Physics/physics_internal.h>
/// ::opgs16::manager::physics::_internal::CPhysicsEnvironment
#include <Manager/Physics/physics_environment.h>

/// import loggers
#include <Headers/import_logger.h>

namespace {
using opgs16::manager::physics::ECollided;
using opgs16::manager::physics::_internal::CCollisionShapeList;
using opgs16::manager::physics::_internal::CPhysicsEnvironment;
using opgs16::manager::physics::_internal::EPhysicsEnvironment;

CPhysicsEnvironment physics_environment {EPhysicsEnvironment::Default};

CCollisionShapeList collision_shape_list;

} /// unnamed namespace

namespace {

///
/// @struct MovementOffset
///
/// @brief
///
///
struct MovementOffset {
  int x = 0;
  int y = 0;
};

///
/// @struct CollisionFlag
///
/// @brief
/// Collision flag structure for AABB.
///
struct CollisionFlag {
  ECollided x        = ECollided::NotCollided;
  ECollided y_top    = ECollided::NotCollided;
  ECollided y_bottom = ECollided::NotCollided;
};

#ifdef false

///
/// @brief
/// Check AABB Collision detection
///
/// @param[in]
/// @param[in]
///
ECollided DetectCollisionAabbExt(
    const opgs16::physics::CRectangleCollider2D* s_collider,
    const opgs16::physics::CRectangleCollider2D* d_collider,
    const MovementOffset offset = {}) {
  using PositionType = opgs16::physics::CRectangleCollider2D::PositionType;
  auto s_m = s_collider->GetTipPosition(PositionType::LEFT_DOWN);
  s_m.x += offset.x;
  s_m.y += offset.y;

  auto s_M = s_collider->GetTipPosition(PositionType::RIGHT_UP);
  s_M.x += offset.x;
  s_M.y += offset.y;

  const auto d_m = d_collider->GetTipPosition(PositionType::LEFT_DOWN);
  const auto d_M = d_collider->GetTipPosition(PositionType::RIGHT_UP);

  if (s_M.x >= d_m.x && d_M.x >= s_m.x && s_M.y >= d_m.y && d_M.y >= s_m.y)
    return ECollided::Collided;
  else
    return ECollided::NotCollided;
}

///
/// @brief
///
/// @param[in] source
/// @param[in] source_collider
/// @param[in] dest_collider
///
void AdjustPosition(opgs16::component::CRigidbody2D* source,
                    opgs16::physics::CRectangleCollider2D* source_collider,
                    opgs16::physics::CRectangleCollider2D* dest_collider) {
  CollisionFlag   collision_flag;
  const auto&     original_movement = source->Movement();
  MovementOffset  final_offset;

  for (auto dir = 0u; dir < 4u; ++dir) {
    // Dir :: top = 0, bottom = 1, left = 2, right = 3
    MovementOffset movement_offset;

    switch (dir) {
    case 0:
      if (original_movement.y < 0)
        continue;
      break;
    case 1:
      if (original_movement.y > 0)
        continue;
      break;;
    case 2:
      if (original_movement.x < 0)
        continue;
      break;
    case 3:
      if (original_movement.x > 0)
        continue;
      break;;
    default: PHITOS_ASSERT(false, "Never enter subroutine."); break;;
    }

    while (
      DetectCollisionAabbExt(source_collider, dest_collider, movement_offset) ==
      ECollided::Collided
      ) {
      switch (dir) {
      case 0: --movement_offset.y; break;
      case 1: ++movement_offset.y; break;
      case 2: --movement_offset.x; break;
      case 3: ++movement_offset.x; break;
      default:
        PHITOS_ASSERT(false, "Never enter this case.");
        break;
      }
    }

    switch (dir) {
    case 0: case 1:
      final_offset.y = movement_offset.y;
      break;
    case 2: case 3:
      final_offset.x = movement_offset.x;
      break;
    default:
      PHITOS_ASSERT(false, "Never enter this case.");
      break;
    }
  }

  if (original_movement.y > 0 && final_offset.y < 0)
    collision_flag.y_top = ECollided::Collided;
  if (original_movement.y < 0 && final_offset.y > 0)
    collision_flag.y_bottom = ECollided::Collided;

  if (final_offset.x <= -1 || final_offset.x >= 1)
    collision_flag.x = ECollided::Collided;

  auto& src_velocity = source->Velocity();
  if (collision_flag.x == ECollided::Collided &&
      src_velocity.y > 0)
    src_velocity.y = 0;

  if (collision_flag.x == ECollided::Collided ||
      collision_flag.y_top == ECollided::Collided ||
      collision_flag.y_bottom == ECollided::Collided) {
    auto& position =
        const_cast<opgs16::DVector3&>(source->GetBindObject().GetWorldPosition());
    position += opgs16::DVector3{
        static_cast<float>(final_offset.x),
        static_cast<float>(final_offset.y), 0.0f };
    position.x = std::floor(position.x);
    position.y = std::floor(position.y);
    source_collider->ReflectPosition(static_cast<glm::vec3>(position));

    if (collision_flag.y_top == ECollided::Collided ||
        collision_flag.y_bottom == ECollided::Collided)
        src_velocity.y = 0;

    if (collision_flag.x == ECollided::Collided)
        src_velocity.x = 0;

    final_offset.x = final_offset.y = 0;
  }
}

#endif

} /// unnamed namespace

namespace opgs16::manager::physics {

void Update(float delta_time) {

}

void RenderCollisionBox() {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

void Clear()
{
}

CPhysicsEnvironment* GetManagement() {
  return &physics_environment;
}

#ifdef false

CCollisionShapeList& GetShapeList() {
  return collision_shape_list;
}

void AddCollider(opgs16::physics::CRectangleCollider2D* const collider, component::CRigidbody2D* const rigidbody)
{
}

#endif

} /// ::opgs16::manager::physics namespace
