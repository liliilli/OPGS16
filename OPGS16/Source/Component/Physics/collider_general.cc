
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/collider_general.cc
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-04 Create file.
///

/// Header file
#include <Component/Physics/collider_general.h>

#include <Element/Internal/physics_collision_shape_wrapper.h>
/// Enhanced import logger
#include <Headers/import_logger.h>
/// ::opgs16::manager::physics
#include <Manager/physics_manager.h>
/// ::opgs16::manager::physics::_internal::CCollisionShapeList
#include <Manager/Internal/physics_collision_shape_list.h>
/// ::opgs16::manager::_internal::CPhysicsEnvironment
#include <Manager/Internal/physics_environment.h>

namespace opgs16::component::physics {

CColliderGeneral::~CColliderGeneral() {
  if (m_motion_state)
    delete m_motion_state;

  if (m_rigidbody != nullptr) {
    if (m_is_rigidbody_bound == ERigidbodyBound::Binded) {
      manager::physics::GetManagement().RemoveRigidbody(m_rigidbody);
    }

    delete m_rigidbody;
  }
}

void CColliderGeneral::AddCollisionShapeWithRigidbody(const char* shape_name) {
  auto [it, result] =
      manager::physics::GetShapeList().FindCollisionShape(shape_name);
  PHITOS_ASSERT(result == EColliderFindResult::Found,
      "Find collision shape is not valid.");
  if (result == EColliderFindResult::NotFound) {
    return;
  }

  m_collision_shape_wrapper = it->second;

  switch (m_object_physics_type) {
  case EDynamic::Dynamic:
    m_collision_shape_wrapper.GetShape()->
        calculateLocalInertia(m_object_mass, m_local_interia);
    break;
  case EDynamic::Kinetic:
  case EDynamic::Static:
    m_object_mass = 0.0f;
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  //!
  //! @todo Renew heap instances if address is not nullptr.
  //! @todo Check CollisionShape count increment.
  //!
  m_motion_state = new btDefaultMotionState(m_transform);
  m_rigidbody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo{
      m_object_mass,
      m_motion_state,
      m_collision_shape_wrapper.GetShape(),
      m_local_interia
  });
  it->second.__SetUsed();
}

void CColliderGeneral::BindRigidbodyToDynamicWorld() {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");
  PHITOS_ASSERT(m_is_rigidbody_bound == ERigidbodyBound::NotBind,
      "Rigidbody is not bind yet.");

  manager::physics::GetManagement().AddRigidbody(m_rigidbody);
  m_is_rigidbody_bound = ERigidbodyBound::Binded;
}

void CColliderGeneral::SetWorldPosition(const DVector3& world_position) {
  m_transform.setOrigin(static_cast<btVector3>(world_position));

  switch (m_object_physics_type) {
  case EDynamic::Dynamic:
  case EDynamic::Kinetic:
    if (m_rigidbody)
      m_rigidbody->setWorldTransform(m_transform);
    break;
  case EDynamic::Static:
    if (m_is_rigidbody_bound == ERigidbodyBound::Binded) {
      PUSH_LOG_INFO("CColliderGeneral is Static collision object.");
    }
    else {
      m_transform.setOrigin(btVector3{world_position});
    }
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

void CColliderGeneral::SetRestitution(float value) {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");

  m_rigidbody->setRestitution(value);
}

void CColliderGeneral::SetMass(float mass_value) {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");
  if (m_object_physics_type == EDynamic::Static) {
    PUSH_LOG_WARN(
        "Static collision shape could not set up mass weight.\n"
        "You have to change collision shape type to Dynamic or Kinetic at first."
    );
    return;
  }
  if (mass_value <= 0.f) {
    PUSH_LOG_WARN_EXT("Could not set up mass to {}", mass_value);
    return;
  }

  m_object_mass = mass_value;

  if (m_object_physics_type == EDynamic::Dynamic) {
    m_rigidbody->getCollisionShape()->
        calculateLocalInertia(m_object_mass, m_local_interia);
  }

  m_rigidbody->setMassProps(m_object_mass, m_local_interia);
}

void CColliderGeneral::SetInteria(const DVector3& interia) {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");

  m_local_interia = static_cast<btVector3>(interia);
  if (m_object_physics_type == EDynamic::Dynamic) {
    m_rigidbody->getCollisionShape()->
        calculateLocalInertia(m_object_mass, m_local_interia);
  }

  m_rigidbody->setMassProps(m_object_mass, m_local_interia);
}

void CColliderGeneral::SetType(EDynamic type) {
  PHITOS_ASSERT(m_is_rigidbody_bound == ERigidbodyBound::NotBind,
      "Rigidbody is not bind yet.");

  m_object_physics_type = type;
}

void CColliderGeneral::SetLinearFactor(EAxis3D axis, Switch value) {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");

  switch (axis) {
  case EAxis3D::X:
    x_fixed = value;
    break;
  case EAxis3D::Y:
    y_fixed = value;
    break;
  case EAxis3D::Z:
    z_fixed = value;
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }

  UpdateLinearFactor();
}

void CColliderGeneral::UpdateLinearFactor() noexcept {
  PHITOS_ASSERT(m_rigidbody != nullptr,
      "Rigidbody is not initialized or null.");

  m_rigidbody->setLinearFactor({
      (x_fixed == Switch::OFF ? 1.f : 0.f),
      (y_fixed == Switch::OFF ? 1.f : 0.f),
      (z_fixed == Switch::OFF ? 1.f : 0.f)
  });
}

} /// ::opgs16::component::physics namespace.
