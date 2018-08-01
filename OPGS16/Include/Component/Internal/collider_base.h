#ifndef OPGS16_COMPONENT_INTERNAL_COLLIDER_BASE_H
#define OPGS16_COMPONENT_INTERNAL_COLLIDER_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/collider_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

#include <Component/Internal/component.h>
#include <Element/Internal/physics_enums.h>
#include <Element/Internal/physics_structs.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CRigidbody2D;
}

namespace opgs16::component::_internal {
class CRigidbodyStaticBase;
class CRigidbodyDynamicBase;
class CRigidbodyBase;
}

//!
//! Implementation
//!

namespace opgs16::component::_internal {

class CColliderBase : public CComponent {
  using EColliderActualType = element::_internal::EColliderActualType;

public:
  CColliderBase(element::CObject& bind_object);
  ~CColliderBase();

  ///
  /// @brief Get this collider instance's unique index value.
  ///
  uint32_t GetUniqueIndexValue() const noexcept;

  EColliderActualType GetColliderType() const noexcept;

  const DLinearLimitFactor& GetLinearFactor() const noexcept;

  ///
  /// @brief Set trigger or collision flag.
  ///
  void SetTriggered(bool is_triggered);

  bool IsTriggered() const noexcept;

protected:
  btRigidBody**      GetLocalRigidbody() const noexcept;
  btCollisionShape** GetCollisionShape() const noexcept;

  float pGetMass() const noexcept;

  void __pUpdateFlags() noexcept;

private:
  virtual void pInitializeCollider() = 0;

  void pSetMass(float mass_value) noexcept;

  void pSetColliderType(EColliderActualType collider_type) noexcept;

  void pSetLinearFactor(const DLinearLimitFactor& linear_factor) noexcept;

  void pSetUsingGravity(bool use_gravity) noexcept;

  mutable btRigidBody*      m_local_rigidbody = nullptr;
  mutable btCollisionShape* m_collision_shape = nullptr;
  _internal::CRigidbodyBase* m_bind_rigidbody = nullptr;

  EColliderActualType m_collider_type = EColliderActualType::None;
  DLinearLimitFactor m_linear_factor = {true, true, true};
  float m_mass = 0.001f;
  float m_actual_mass = m_mass;

  bool  m_is_use_gravity = true;
  bool  m_is_collision_triggered  = false;

  uint32_t m_collider_index = 0;

  friend opgs16::component::CRigidbody2D;
  friend opgs16::component::_internal::CRigidbodyBase;
  friend opgs16::component::_internal::CRigidbodyDynamicBase;
  friend opgs16::component::_internal::CRigidbodyStaticBase;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CColliderBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_COLLIDER_BASE_H