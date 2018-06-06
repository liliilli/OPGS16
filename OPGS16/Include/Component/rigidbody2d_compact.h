#ifndef OPGS16_COMPONENT_RIGIDBODY2D_COMPACT_H
#define OPGS16_COMPONENT_RIGIDBODY2D_COMPACT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/rigidbody2d_compact.h
///
/// @brief
/// Integrated rigidoby and collider component.
///
/// @author Jongmin Yun
/// @log
/// 2018-06-06 Create file.
///

#include <string>

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h>

#include <Helper/switch.h>

#include <Helper/Physics/enum_types.h>

#include <Manager/Internal/physics_collision_shape_list.h>

//!
//! Forward declaration.
//!

namespace opgs16::manager::physics {
_internal::CCollisionShapeList& GetShapeList();
}

//!
//! Implementation.
//!

namespace opgs16::component {

class CRigidbody2DCompact final : public _internal::CComponent {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRigidbody2DCompact)
public:
  CRigidbody2DCompact(element::CObject& bind_object) : CComponent(bind_object) {
    m_transform.setIdentity();
  };

  ///
  ///
  ~CRigidbody2DCompact();

  ///
  /// @brief
  /// Generate and bind temporary collider to rigidbody.
  ///
  template <
    EColliderType TType,
    typename... TParams
  >
  decltype(auto) AddCollider2D(TParams&&... args) {
    static_assert(
        TType != EColliderType::None, "Collider type must not be None.");
    static_assert(
        (TType != EColliderType::Box3D) || (TType != EColliderType::Sphere),
        "Collider type must be 2D types");

    auto& shape_list = manager::physics::GetShapeList();
    if (m_is_collider_bound == ERigidbodyBound::Binded) {
      if (m_collider_hash_name.empty())
        PHITOS_UNEXPECTED_BRANCH();

      shape_list.RemoveCollisionShape(m_collider_hash_name.c_str());
      m_is_collider_bound = ERigidbodyBound::NotBind;
    }

    m_collider_hash_name = GetTemporaryColliderName();
    ++m_value;

    auto [it, type] = shape_list.TryEmplaceCollisionShape<TType>(
      m_collider_hash_name.c_str(),
      std::forward<TParams>(args)...);
    if (type == EColliderReturnType::Failed)
      PHITOS_UNEXPECTED_BRANCH();

    m_is_collider_bound = ERigidbodyBound::Binded;
  }

private:
  void Update() override final;

  ///
  /// @brief
  /// Make temporal collider tag name and return.
  ///
  /// @return Temporary collider name.
  ///
  std::string GetTemporaryColliderName();

  std::string m_collider_hash_name = "";
  uint32_t    m_value = 0;

  float m_mass = 0.0f;
  float m_drag = 0.0f;
  float m_angular_drag = 0.0f;

  EDynamic m_object_physics_type = EDynamic::Static;
  Switch x_fixed = Switch::OFF;
  Switch y_fixed = Switch::OFF;

  btRigidBody* m_rigidbody = nullptr;
  btDefaultMotionState* m_motion_state = nullptr;
  btTransform m_transform = btTransform{};

  mutable ERigidbodyBound m_is_collider_bound = ERigidbodyBound::NotBind;
  mutable Switch m_use_gravity = Switch::ON;
};

}

#endif /// OPGS16_COMPONENT_RIGIDBODY2D_COMPACT_H