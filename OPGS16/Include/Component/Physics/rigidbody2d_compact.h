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

#include <limits>
#include <string>

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h>

#include <Helper/switch.h>

#include <Helper/Physics/enum_types.h>

#include <Manager/Physics/physics_collision_shape_list.h>

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
    UpdateTransform();
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

    m_collision_shape_wrapper = it->second;
    switch (m_object_physics_type) {
    case EDynamic::Dynamic:
      if (m_mass == 0.0f)
        m_mass = std::numeric_limits<float>::min();

      m_collision_shape_wrapper.GetShape()->
          calculateLocalInertia(m_mass, m_local_interia);
      break;
    case EDynamic::Kinetic:
    case EDynamic::Static:
      m_mass = 0.0f;
      break;
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    }

    //!
    //! @todo Renew heap instances if address is not nullptr.
    //! @todo Check CollisionShape count increment.
    //!
    m_motion_state = new btDefaultMotionState(m_transform);
    m_rigidbody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo{
        m_mass,
        m_motion_state,
        m_collision_shape_wrapper.GetShape(),
        m_local_interia
    });
    it->second.__SetUsed();
    m_is_collider_bound = ERigidbodyBound::Binded;
  }

private:
  void Update(float delta_time) override final;

  ///
  /// @brief
  /// Update transform, retriving final position, scale, rotation of bound object.
  ///
  void UpdateTransform();

  ///
  /// @brief
  /// Make temporal collider tag name and return.
  ///
  /// @return Temporary collider name.
  ///
  std::string GetTemporaryColliderName();

  ///
  /// @brief
  ///


  std::string m_collider_hash_name = "";
  uint32_t    m_value = 0;

  float m_mass = 0.0f;
  float m_interia = 0.0f;
  float m_drag = 0.0f;
  float m_angular_drag = 0.0f;

  EDynamic m_object_physics_type = EDynamic::Static;
  Switch x_fixed = Switch::OFF;
  Switch y_fixed = Switch::OFF;

  //!
  //! Not controlable
  //!

  btVector3 m_local_interia = btVector3{m_interia, m_interia, m_interia};

  btRigidBody* m_rigidbody = nullptr;
  btDefaultMotionState* m_motion_state = nullptr;
  btTransform m_transform = btTransform{};
  element::_internal::CCollisionShapeWrapper m_collision_shape_wrapper;

  mutable ERigidbodyBound m_is_collider_bound = ERigidbodyBound::NotBind;
  mutable Switch m_use_gravity = Switch::ON;
};

}

#endif /// OPGS16_COMPONENT_RIGIDBODY2D_COMPACT_H