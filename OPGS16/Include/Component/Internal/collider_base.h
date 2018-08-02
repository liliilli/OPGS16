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

#include <Component/Internal/aabb_renderer_2d.h>
#include <Component/Internal/component.h>
#include <Element/Internal/physics_enums.h>
#include <Element/Internal/physics_structs.h>
#include <Helper/Type/vector3.h>
#include <Element/Internal/physics_collider_bind_info.h>

//!
//! Forward declaration
//!

namespace opgs16::manager::physics::_internal {
class CPhysicsEnvironment;
}

namespace opgs16::component {
class CRigidbodyStatic2D;
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
  using EColliderBehaviorState = element::_internal::EColliderBehaviorState;
  using EColliderCollisionState = element::_internal::EColliderCollisionState;
  using TAabbRendererSmtPtr = std::unique_ptr<_internal::CPrivateAabbRenderer2D>;

public:
  ///
  /// Bind collider instance to unique rigidbody of CObject.
  /// and, Assign unique ID number for collider identification.
  ///
  CColliderBase(element::CObject& bind_object);

  ///
  /// When release instance, unbind from rigidbody and release from physics world.
  ///
  ~CColliderBase();

  CColliderBase(const CColliderBase&) = default;
  CColliderBase(CColliderBase&&)      = default;
  CColliderBase& operator=(const CColliderBase&)  = default;
  CColliderBase& operator=(CColliderBase&&)       = default;

  ///
  /// @brief Get this collider instance's unique index value.
  ///
  uint32_t GetUniqueIndexValue() const noexcept;

  EColliderActualType GetColliderType() const noexcept;

  EColliderBehaviorState GetBehaviorState() const noexcept;

  const DLinearLimitFactor& GetLinearFactor() const noexcept;

  ///
  /// @brief Set trigger / collision function calling flag.
  ///
  void SetTriggered(bool is_triggered);

  bool IsTriggered() const noexcept;

protected:
  btRigidBody**      GetLocalRigidbody() const noexcept;
  btCollisionShape** GetCollisionShape() const noexcept;

  void pfSetBehaviorState(EColliderBehaviorState state) noexcept;

  float pGetMass() const noexcept;

  ///
  /// Check any collision on previous frame has been caused or not,
  /// so to do proceed specific procedure.
  ///
  void Update(float delta_time) override;

  ///
  /// Update collider flags when rigidbody types becomes
  /// Kinetic / Dynamic and Static.
  ///
  void pUpdateColliderTypeFlag() noexcept;

  ///
  /// Create Aabb renderer 2d or 3d.
  ///
  bool pInitiateAabbRenderer(bool is_2d);

  ///
  /// 
  ///
  void pSetColliderUserPointerAndBind();

private:
  virtual void pInitializeCollider() = 0;

  void pSetMass(float mass_value) noexcept;

  void pSetColliderType(EColliderActualType collider_type) noexcept;

  void pSetLinearFactor(const DLinearLimitFactor& linear_factor) noexcept;

  void pSetUsingGravity(bool use_gravity) noexcept;

  ///
  /// @brief Update aabb information
  /// This function must be called in CPhysicsEnvironment.
  ///
  void pfUpdateAabbToRenderer(const DVector3& min, const DVector3& max);

  ///
  /// @brief Call bind object's collision or trigger callback function.
  ///
  void pfCallBindObjectCallback(CColliderBase* other_collider);

  ///
  /// @brief Check if callback function is already called on this frame.
  ///
  bool pfIsCallbackFunctionCalledOnThisFrame() const noexcept;

  TAabbRendererSmtPtr m_aabb_renderer = nullptr;

  mutable btRigidBody*      m_local_rigidbody = nullptr;
  mutable btCollisionShape* m_collision_shape = nullptr;
  _internal::CRigidbodyBase* m_bind_rigidbody = nullptr;

  EColliderActualType    m_collider_type  = EColliderActualType::None;
  EColliderBehaviorState m_behavior_state = EColliderBehaviorState::None;
  EColliderCollisionState m_collision_state = EColliderCollisionState::Idle;
  DLinearLimitFactor     m_linear_factor  = {true, true, true};

  element::_internal::DPrivateColliderBindInfo m_bind_info;

  float m_mass        = 0.001f;
  float m_actual_mass = m_mass;

  bool  m_is_use_gravity = true;
  bool  m_is_collision_triggered = false;
  bool  m_is_collided_on_this_frame = false;

  uint32_t m_collider_index = 0;

  friend opgs16::component::CRigidbody2D;
  friend opgs16::component::CRigidbodyStatic2D;
  friend opgs16::component::_internal::CRigidbodyBase;
  friend opgs16::component::_internal::CRigidbodyDynamicBase;
  friend opgs16::component::_internal::CRigidbodyStaticBase;
  friend opgs16::manager::physics::_internal::CPhysicsEnvironment;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CColliderBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_COLLIDER_BASE_H