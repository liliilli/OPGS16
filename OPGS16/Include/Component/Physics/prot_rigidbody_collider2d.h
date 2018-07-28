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

#include <Component/Internal/component.h>

#include <string>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Helper/Type/vector3.h"

namespace opgs16::component {

class CProtoRigidbodyCollider2D final : public _internal::CComponent {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CProtoRigidbodyCollider2D)
public:
  CProtoRigidbodyCollider2D(
      element::CObject& bind_object,
      const float mass_sum,
      const float is_kinematic,
      const DVector2& collider_size,
      const std::string& collision_tag);

  ~CProtoRigidbodyCollider2D();

  void SetMass(float mass_value);

  void SetKinematic(bool is_kinematic);

  ///
  /// @update
  /// Frequent call will cause performance decrease.
  ///
  void SetColliderSize(const DVector2& size);

  float GetMass() noexcept;

  float IsKinematic() noexcept;

private:
  void Update(float delta_time) override final;

  btRigidBody* m_rigidbody = nullptr;
  btCollisionShape* m_collision_shape = nullptr;

  int32_t m_collision_tag_index = -1;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H