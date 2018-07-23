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
#include <Component/rigidbody_2d.h>
/// collision::CRectangleCollider2D
#include <Component/Physics2D/Collider/rectangle.h>
/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::manager::MSettingManager
#include <Manager/setting_manager.h>
/// ::opgs16::manager::physics::_internal::CCollisionShapeList
#include <Manager/Internal/physics_collision_shape_list.h>
/// ::opgs16::manager::physics Strong enum flags.
#include <Manager/Internal/physics_flag.h>
/// ::opgs16::manager::_internal::Item
#include <Manager/Internal/physics_internal.h>
/// ::opgs16::manager::physics::_internal::CPhysicsEnvironment
#include <Manager/Internal/physics_environment.h>


/// import loggers
#include <Headers/import_logger.h>

using ECollided = opgs16::manager::physics::ECollided;
using CCollisionShapeList = opgs16::manager::physics::_internal::CCollisionShapeList;
using CPhysicsEnvironment = opgs16::manager::physics::_internal::CPhysicsEnvironment;
using EPhysicsEnvironment = opgs16::manager::physics::_internal::EPhysicsEnvironment;

namespace {

using item_ptr = std::unique_ptr<opgs16::manager::_internal::Item>;
using item_raw = opgs16::manager::_internal::Item* ;

std::vector<item_ptr> m_potential;
std::vector<item_raw> m_active;

//
CPhysicsEnvironment management{EPhysicsEnvironment::Default};
//
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


///
/// @brief
/// Proceed AABB collision check for Rigidbody2D item for activated rigidbody2d
/// collidable objects.
///
/// If item and any object is collided each other, call OnCollisionEnter or
/// OnTriggerEnter itself with parameter as objective's rigidbody reference.
///
/// @param[in] item Collision item object.
///
void ProceedCollisionCheck(item_ptr& item) {
  for (auto& active_item : m_active) {
    // If rigidbody is same, do not check collision.
    auto const s_rigidbody = active_item->m_rigidbody;
    auto const d_rigidbody = item->m_rigidbody;
    if (s_rigidbody == d_rigidbody)
      continue;

    auto const s_collider = active_item->m_collider;
    auto const d_collider = item->m_collider;
    if (!opgs16::manager::setting::CollisionLayerCheck(
        s_collider->CollisionLayer(), d_collider->CollisionLayer()))
      continue;

    if (DetectCollisionAabbExt(s_collider, d_collider) == ECollided::Collided) {
      // Check solidification
      // Disable adjusting position temporarily because it's so much buggy.
      // @todo Fix AdjustPosition.
#ifdef false
      if (d_rigidbody->IsSolid() && !s_rigidbody->IsSolid()) {
        AdjustPosition(s_rigidbody, s_collider, d_collider);
      }
      else if (s_rigidbody->IsSolid() && !d_rigidbody->IsSolid()) {
        AdjustPosition(d_rigidbody, d_collider, s_collider);
      }
#endif

      // Call callback member function
      using CollisionType = opgs16::physics::CCollider2D::ECollisionType;
      if (s_collider->CollisionType() == CollisionType::COLLISION)
        s_rigidbody->OnCollisionEnter(*d_rigidbody);
      else
        s_rigidbody->OnTriggerEnter(*d_rigidbody);

      if (d_collider->CollisionType() == CollisionType::COLLISION)
        d_rigidbody->OnCollisionEnter(*s_rigidbody);
      else
        d_rigidbody->OnTriggerEnter(*s_rigidbody);
    }
  }
}

void EraseItem(item_ptr& item) {
  for (auto it = m_active.cbegin(); it != m_active.cend(); ++it) {
    if ((*it)->m_collider == item->m_collider) {
      m_active.erase(it);
      break;
    }
  }
}

} /// unnamed namespace

namespace opgs16::manager::physics {

void AddCollider(opgs16::physics::CRectangleCollider2D* const collider,
  component::CRigidbody2D* const rigidbody) {
  using Type = opgs16::physics::CRectangleCollider2D::PositionType;
  auto ld = collider->GetTipPosition(Type::LEFT_DOWN);
  auto ru = collider->GetTipPosition(Type::RIGHT_UP);

  // Insert to potential list
  m_potential.emplace_back(
    std::make_unique<manager::_internal::Item>(collider, rigidbody, ld,
                                      manager::_internal::Item::Type::Begin));
  m_potential.emplace_back(
    std::make_unique<manager::_internal::Item>(collider, rigidbody, ru,
                                      manager::_internal::Item::Type::End));
}

void Update() {
  // Sorting lambda function (do not stable_sort).
  std::sort(m_potential.begin(), m_potential.end(),
      [](const item_ptr& lhs, const item_ptr& rhs) {
      return lhs->m_position.x < rhs->m_position.x;
  });

  // Processing
  for (auto& item : m_potential) {
    switch (item->m_type) {
    case manager::_internal::Item::Type::Begin:
      ProceedCollisionCheck(item);
      m_active.emplace_back(item.get());
      break;
    case manager::_internal::Item::Type::End:
      EraseItem(item);
      break;
    }
  }

  m_active.clear();
}

void RenderCollisionBox() {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

void Clear() {
  m_active.clear();
  m_potential.clear();
}

CPhysicsEnvironment& GetManagement() {
  return management;
}

CCollisionShapeList& GetShapeList() {
  return collision_shape_list;
}

} /// ::opgs16::manager::physics namespace
