#include <precompiled.h>
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

/// Header file
#include <Component/Physics/prot_rigidbody_collider2d.h>

#include <BulletCollision/CollisionShapes/btBox2dShape.h>

#include <Core/core_setting.h>
#include <Component/Internal/aabb_renderer_2d.h>
#include <Element/object.h>
#include <Manager/physics_manager.h>

namespace opgs16::component {

CProtoRigidbodyCollider2D::CProtoRigidbodyCollider2D(
    element::CObject& bind_object,
    const float mass_sum,
    const float is_kinematic,
    const DVector2& collider_size,
    const std::string& collision_tag) :
    CComponent{ bind_object } {
  pCreateRigidbody(collider_size, mass_sum, &m_rigidbody);
  // Set Kinematic or Dynamic.
  if (is_kinematic) {
    m_rigidbody->setCollisionFlags(
        m_rigidbody->getCollisionFlags() |
        btCollisionObject::CF_KINEMATIC_OBJECT
    );
    m_rigidbody->setActivationState(DISABLE_DEACTIVATION);
  }

  opgs16::manager::physics::AddRigidbody(m_rigidbody);

  // Create CPrivateAabbRenderer2D and set information.
  auto& obj = GetBindObject();
  m_aabb_renderer = std::make_unique<_internal::CPrivateAabbRenderer2D>(obj);
  m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(collider_size));
  m_aabb_renderer->SetCollisionRenderPosition(obj.GetFinalPosition());
}

void CProtoRigidbodyCollider2D::pCreateRigidbody(
    const DVector2& collider_size,
    const float mass_sum,
    btRigidBody** rigidbody_ptr) {
  using opgs16::element::_internal::EDirection;
  auto& obj = GetBindObject();

  // Create collision shape
  const DVector2 half_size = collider_size * 0.5f;
  m_collision_shape = new btBoxShape{{half_size.x, half_size.y, 1.f}};
  m_collision_shape->setMargin(1.f);

  // Create motion state
  btQuaternion rotation;
  rotation.setEulerZYX(obj.GetRotationWpAngle(EDirection::Z),
                       obj.GetRotationWpAngle(EDirection::Y),
                       obj.GetRotationWpAngle(EDirection::X));
  btDefaultMotionState* motionState =
      new btDefaultMotionState(btTransform{rotation, obj.GetFinalPosition()});

  btVector3 local_inertia;
  m_collision_shape->calculateLocalInertia(mass_sum, local_inertia);

  // Create rigidbody info
  btRigidBody::btRigidBodyConstructionInfo body_construction_info =
      btRigidBody::btRigidBodyConstructionInfo{
          mass_sum,
          motionState,
          m_collision_shape,
          local_inertia};
  body_construction_info.m_restitution = 0.0f;
  body_construction_info.m_friction = 1.0f;

  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setUserPointer(static_cast<void*>(&obj));

  // Restrict physical influence to (x, y) axis only.
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});
}

CProtoRigidbodyCollider2D::~CProtoRigidbodyCollider2D() {
  using opgs16::manager::physics::RemoveRigidbody;
  opgs16::manager::physics::RemoveRigidbody(m_rigidbody);

  if (m_rigidbody) {
    delete m_rigidbody->getMotionState();
    delete m_rigidbody;
  }

  delete m_collision_shape;
}

void CProtoRigidbodyCollider2D::Update(float delta_time) {
  auto& obj = GetBindObject();
  const auto& position = obj.GetFinalPosition();

  if (m_is_position_initialized) {
    btTransform transform;
    m_rigidbody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(position);
    m_rigidbody->getMotionState()->setWorldTransform(transform);
    m_is_position_initialized = true;
  }

  // Debug
  const DVector3 center { m_rigidbody->getCenterOfMassPosition() };
  btVector3 min;
  btVector3 max;
  m_rigidbody->getAabb(min, max);
  PUSH_LOG_DEBUG_EXT(
      "{} center is ({}, {}, {}) / AABB ({}, {}, {}) ~ ({}, {}, {})",
      obj.GetGameObjectName(),
      center.x, center.y, center.z,
      min.x(), min.y(), min.z(),
      max.x(), max.y(), max.z());
  // Debug end

  if (m_aabb_renderer) {
    m_aabb_renderer->SetCollisionRenderPosition(position);
    m_aabb_renderer->Update(delta_time);

    opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
  }
}

void CProtoRigidbodyCollider2D::SetMass(float mass_value) {
  if (mass_value <= 0) {
    mass_value = 0.001f;
  }

  btVector3 local_inertia;
  m_collision_shape->calculateLocalInertia(mass_value, local_inertia);
  m_rigidbody->setMassProps(mass_value, local_inertia);
}

void CProtoRigidbodyCollider2D::SetKinematic(bool is_kinematic) {
  if (is_kinematic) {
    m_rigidbody->setCollisionFlags(
        m_rigidbody->getCollisionFlags() |
        btCollisionObject::CF_KINEMATIC_OBJECT
    );
  }
  else {
    m_rigidbody->setCollisionFlags(
        m_rigidbody->getCollisionFlags() |
        btCollisionObject::CF_CHARACTER_OBJECT
    );
  }
}

void CProtoRigidbodyCollider2D::TemporalSetStatic() {
  m_rigidbody->setMassProps(0, {});
}

void CProtoRigidbodyCollider2D::SetColliderSize(const DVector2& size) {
  if (m_collision_shape)
    delete m_collision_shape;

  m_collision_shape = new btBox2dShape{
      static_cast<btVector3>(size * 0.5f)
  };
  m_rigidbody->setCollisionShape(m_collision_shape);
  m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(size));
}

float CProtoRigidbodyCollider2D::GetMass() noexcept {
  return m_rigidbody->getInvMass();
}

float CProtoRigidbodyCollider2D::IsKinematic() noexcept {
  return m_rigidbody->isKinematicObject();
}

} /// ::opgs16::component namespace