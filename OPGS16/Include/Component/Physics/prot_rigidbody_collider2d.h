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

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <Component/Internal/component.h>
#include <Element/Internal/physics_collider_bind_info.h>
#include <Helper/Type/vector2.h>
#include "Element/Internal/physics_enums.h"

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

class CProtoRigidbodyCollider2D final : public _internal::CComponent {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CProtoRigidbodyCollider2D)
  using EColliderActualType = element::_internal::EColliderActualType;
  using EColliderBehaviorState = element::_internal::EColliderBehaviorState;
  using EColliderCollisionState = element::_internal::EColliderCollisionState;
  using TAabbRendererSmtPtr = std::unique_ptr<_internal::CPrivateAabbRenderer2D>;

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

  void TemporalSetStatic();

  void SetTriggered(bool is_triggered);

  ///
  /// @update
  /// Frequent call will cause performance decrease.
  ///
  void SetColliderSize(const DVector2& size);

  float GetMass() const noexcept;

  EColliderBehaviorState GetColliderState() const noexcept;

  EColliderActualType GetColliderActualType() const noexcept;

  bool IsKinematic() const noexcept;

  bool IsTriggered() const noexcept;

private:
  void Update(float delta_time) override final;

  void pCreateRigidbody(const DVector2& collider_size,
                        const float mass_sum,
                        btRigidBody** rigidbody_ptr);
  void pUpdateAabbToRenderer(const DVector3& min, const DVector3& max);

  void pSetCollisionState(EColliderBehaviorState state);

  void pSetCollisionActualType(EColliderActualType type);

  ///
  /// @brief
  ///
  void pCallBindObjectCallback(CProtoRigidbodyCollider2D* other_collider);

  bool pIsCallbackFunctionCalledOnThisFrame() const noexcept;

  btRigidBody*      m_rigidbody       = nullptr;
  btCollisionShape* m_collision_shape = nullptr;
  TAabbRendererSmtPtr m_aabb_renderer = nullptr;

  element::_internal::DPrivateColliderBindInfo m_bind_info;
  EColliderBehaviorState m_state      = EColliderBehaviorState::None;
  EColliderActualType m_collider_type = EColliderActualType::None;
  EColliderCollisionState m_collision_state = EColliderCollisionState::Idle;

  int32_t m_collision_tag_index = -1;

  bool m_is_position_initialized = false;
  bool m_is_collided_flag_setup  = false;
  bool m_is_collision_triggered  = false;

  friend opgs16::manager::physics::_internal::CPhysicsEnvironment;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_PROT_RIGIDBODY_COLLIDER2D_H