#ifndef OPGS16_COMPONENT_PHYSICS_COLLIDER_GENERAL_H
#define OPGS16_COMPONENT_PHYSICS_COLLIDER_GENERAL_H

///
/// @file Component/Physics/collider_general.h
///
/// @brief
/// Implementation file of collision shape object.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-02 Create file.
///

/// Bullet library include
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

/// Enhanced assertion
#include <Phitos/Dbg/assert.h>

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
/// Component Macroes
#include <Component/Internal/component_macro.h>
/// ::opgs16::element::_internal::CCollisionShapeWrapper
#include <Element/Internal/physics_collision_shape_wrapper.h>
/// ::opgs16::EAxis3D
#include <Helper/Type/axis.h>
/// ::opgs16:: Enum types for physics
#include <Helper/Physics/enum_types.h>
/// ::opgs16::ESwitch
#include <Helper/switch.h>
/// ::opgs16::DVector types
#include <Helper/include_vector.h>
/// ::opgs16::manager::physics
#include <Manager/physics_manager.h>
/// ::opgs16::manager::physics::_internal::CPhysicsEnvironment
#include <Manager/Physics/physics_environment.h>
/// ::opgs16::manager::physics::_internal::CCollisionShapeList
#include <Manager/Physics/physics_collision_shape_list.h>

namespace opgs16::component::physics {

///
/// @class CColliderGeneral
///
/// @brief
/// Collision shape object which wraps bullet collision shapes
///
class CColliderGeneral : public _internal::CComponent {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CColliderGeneral)
public:
  CColliderGeneral(element::CObject& bind_object) :
      _internal::CComponent(bind_object) {
    m_transform.setIdentity();
  }

  ~CColliderGeneral();

  ///
  /// @brief
  /// Add valid collision shape and activate CColliderGeneral instance.
  /// If not find collision shape with shape name, do nothing.
  ///
  /// @param[in] shape_name Collision shape name to bind.
  ///
  void AddCollisionShapeWithRigidbody(const char* shape_name);

  ///
  /// @brief
  ///
  /// @param[in] collider_type
  /// @param[in] shape_name
  /// @param[in] args
  ///
  /// @tparam TArgs
  ///
  template <EColliderType TType, typename... TArgs>
  void AddCollisionShapeWithRigidbody(const char* shape_name,
                                      TArgs&&... args) {
    auto& shape_list = manager::physics::GetShapeList();
    const auto result = shape_list.IsCollisionExist(shape_name);
    if (result == EColliderFindResult::Found) {
      AddCollisionShapeWithRigidbody(shape_name);
      return;
    }

    auto [it, type] = shape_list.TryEmplaceCollisionShape<TType>(
        shape_name,
        std::forward<TArgs>(args)...);
    if (type == EColliderReturnType::Failed)
      PHITOS_UNEXPECTED_BRANCH();
    AddCollisionShapeWithRigidbody(shape_name);
  }

  ///
  /// @brief
  /// Add generated rigidbody with valid collision shape into dynamic world.
  ///
  void BindRigidbodyToDynamicWorld();

  ///
  /// @brief
  /// Set final world position.
  ///
  /// @param[in] world_position World position
  ///
  void SetWorldPosition(const DVector3& world_position);

  ///
  /// @brief
  /// Set restitution.
  ///
  /// @param[in] value
  ///
  void SetRestitution(float value);

  ///
  /// @brief
  /// Set mass.
  ///
  /// @param[in] mass_value
  ///
  void SetMass(float mass_value);

  ///
  /// @brief
  /// Set interia.
  ///
  /// @param[in]
  ///
  void SetInteria(const DVector3& interia);

  ///
  /// @brief
  /// Set type before calling add collision shape with rigidbody.
  ///
  /// @param[in] type
  ///
  void SetType(EDynamic type);

  ///
  /// @brief
  ///
  ///
  /// @param[in] axis
  /// @param[in] value
  ///
  void SetLinearFactor(EAxis3D axis, Switch value);

private:
  ///
  /// @brief
  /// Update linear factor following xyz_fixed switch.
  ///
  void UpdateLinearFactor() noexcept;

  EDynamic m_object_physics_type = EDynamic::Static;
  ERigidbodyBound m_is_rigidbody_bound = ERigidbodyBound::NotBind;

  btRigidBody* m_rigidbody = nullptr;
  btDefaultMotionState* m_motion_state = nullptr;

  btScalar m_object_mass = 0.f;
  btVector3 m_local_interia {0, 0, 0};
  btTransform m_transform = btTransform();

  element::_internal::CCollisionShapeWrapper m_collision_shape_wrapper;

  Switch x_fixed = Switch::OFF;
  Switch y_fixed = Switch::OFF;
  Switch z_fixed = Switch::OFF;
};

} /// ::opgs16::component::physics Gnamespace.

#endif /// OPGS16_COMPONENT_PHYSICS_COLLIDER_GENERAL_H
