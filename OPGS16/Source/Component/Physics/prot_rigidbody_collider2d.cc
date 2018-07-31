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

#define OP16_SAFE_DELETE(__MAInstance__) \
  delete __MAInstance__; \
  __MAInstance__ = nullptr

namespace opgs16::component {

CProtoRigidbodyCollider2D::CProtoRigidbodyCollider2D(
    element::CObject& bind_object,
    const float mass_sum,
    const float is_kinematic,
    const DVector2& collider_size,
    const std::string& collision_tag) :
    CComponent{ bind_object } {
  auto& obj = GetBindObject();
  m_bind_info.bind_object   = &obj;
  m_bind_info.bind_collider = this;
  pCreateRigidbody(collider_size, mass_sum, &m_rigidbody);
  pSetCollisionActualType(EColliderActualType::Dynamic);
  pSetCollisionState(EColliderBehaviorState::Activated);

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
  m_aabb_renderer = std::make_unique<_internal::CPrivateAabbRenderer2D>(obj, this);
  m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(collider_size));
  m_aabb_renderer->SetCollisionRenderPosition(obj.GetFinalPosition());
}

void CProtoRigidbodyCollider2D::pCreateRigidbody(
    const DVector2& collider_size,
    const float mass_sum,
    btRigidBody** rigidbody_ptr) {
  auto& obj = GetBindObject();

  // Create collision shape
  const DVector2 half_size = collider_size * 0.5f;
  m_collision_shape = new btBox2dShape{{half_size.x, half_size.y, 0.f}};
  m_collision_shape->setMargin(1.f);

  // Create motion state
  btQuaternion rotation;
  rotation.setEulerZYX(obj.GetRotationWpAngle(EAxis3D::Z),
                       obj.GetRotationWpAngle(EAxis3D::Y),
                       obj.GetRotationWpAngle(EAxis3D::X));
  btDefaultMotionState* motionState =
      new btDefaultMotionState(btTransform{rotation, obj.GetFinalPosition()});

  btVector3 local_inertia;
  //m_collision_shape->calculateLocalInertia(mass_sum, local_inertia);

  // Create rigidbody info
  btRigidBody::btRigidBodyConstructionInfo body_construction_info =
      btRigidBody::btRigidBodyConstructionInfo{
          (mass_sum <= 0.f) ? 0.001f : mass_sum,
          motionState,
          m_collision_shape,
          local_inertia};
  body_construction_info.m_restitution = 0.75f;
  body_construction_info.m_friction = 1.0f;

  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setUserPointer(static_cast<void*>(&m_bind_info));

  // Restrict physical influence to (x, y) axis only.
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});
}

CProtoRigidbodyCollider2D::~CProtoRigidbodyCollider2D() {
  using opgs16::manager::physics::RemoveRigidbody;
  RemoveRigidbody(m_rigidbody);

  if (m_rigidbody) {
    delete m_rigidbody->getMotionState();
    OP16_SAFE_DELETE(m_rigidbody);
  }

  OP16_SAFE_DELETE(m_collision_shape);
}

void CProtoRigidbodyCollider2D::Update(float delta_time) {
  auto& obj = GetBindObject();
  const auto& position = obj.GetFinalPosition();

  if (!m_is_position_initialized) {
    btTransform transform;
    m_rigidbody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(position);

    m_rigidbody->setWorldTransform(transform);
    m_rigidbody->getMotionState()->setWorldTransform(transform);
    m_is_position_initialized = true;
  }

  if (m_is_collided_flag_setup) {
    pSetCollisionState(EColliderBehaviorState::Activated);
    m_is_collided_flag_setup = false;
  }
  else {
    if (m_collision_state != EColliderCollisionState::Idle) {
      // @todo temporary
      pCallBindObjectCallback(nullptr);
    }
  }

  if (!m_rigidbody->isActive() &&
      m_collider_type != EColliderActualType::Staic) {
    pSetCollisionState(EColliderBehaviorState::Sleep);
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
    m_collider_type = EColliderActualType::Kinetic;
  }
  else {
    m_rigidbody->setCollisionFlags(
        m_rigidbody->getCollisionFlags() |
        btCollisionObject::CF_CHARACTER_OBJECT
    );
    m_collider_type = EColliderActualType::Dynamic;
  }
}

void CProtoRigidbodyCollider2D::TemporalSetStatic() {
  m_rigidbody->setMassProps(0, {});
  m_collider_type = EColliderActualType::Staic;
}

void CProtoRigidbodyCollider2D::SetTriggered(bool is_triggered) {
  if (is_triggered) {
    if (m_rigidbody->getCollisionFlags() &
        btCollisionObject::CF_NO_CONTACT_RESPONSE) {
      return;
    }

    m_rigidbody->setCollisionFlags(m_rigidbody->getCollisionFlags() |
        btCollisionObject::CF_NO_CONTACT_RESPONSE);
  }
  else if (m_rigidbody->getCollisionFlags() &
           btCollisionObject::CF_NO_CONTACT_RESPONSE) {
    m_rigidbody->setCollisionFlags(
        m_rigidbody->getCollisionFlags() ^
        btCollisionObject::CF_NO_CONTACT_RESPONSE
    );
  }

  m_is_collision_triggered = is_triggered;
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

float CProtoRigidbodyCollider2D::GetMass() const noexcept {
  return m_rigidbody->getInvMass();
}

CProtoRigidbodyCollider2D::EColliderBehaviorState
CProtoRigidbodyCollider2D::GetColliderState() const noexcept {
  return m_state;
}

CProtoRigidbodyCollider2D::EColliderActualType
CProtoRigidbodyCollider2D::GetColliderActualType() const noexcept {
  return m_collider_type;
}

bool CProtoRigidbodyCollider2D::IsKinematic() const noexcept {
  return m_collider_type == EColliderActualType::Kinetic;
}

bool CProtoRigidbodyCollider2D::IsTriggered() const noexcept {
  return m_is_collision_triggered;
}

void CProtoRigidbodyCollider2D::pUpdateAabbToRenderer(const DVector3& min,
                                                      const DVector3& max) {
  if (m_aabb_renderer) {
    m_aabb_renderer->SetCollisionSize(max - min);
    m_aabb_renderer->SetCollisionRenderPosition((max + min) / 2);
    m_aabb_renderer->Update(0);
    opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
  }
}

void CProtoRigidbodyCollider2D::pSetCollisionActualType(EColliderActualType type) {
  PHITOS_ASSERT(type != EColliderActualType::None,
                "Type must not be EColliderActualType::None.");

  m_collider_type = type;
}

void CProtoRigidbodyCollider2D::pSetCollisionState(EColliderBehaviorState state) {
  PHITOS_ASSERT(state != EColliderBehaviorState::None,
                "Collision state must not be None.");

  if (state == EColliderBehaviorState::Collided) {
    m_is_collided_flag_setup = true;
  }

  m_state = state;
}

void CProtoRigidbodyCollider2D::pCallBindObjectCallback(CProtoRigidbodyCollider2D* other_collider) {
  const bool is_collision_function =
      !IsTriggered() &&
      other_collider ? !other_collider->IsTriggered() : true;

  switch (m_collision_state) {
  case EColliderCollisionState::Idle:
    m_collision_state = EColliderCollisionState::Enter;
    break;
  case EColliderCollisionState::Enter:
    if (m_is_collided_flag_setup)
      m_collision_state = EColliderCollisionState::Stay;
    else
      m_collision_state = EColliderCollisionState::Idle;
    break;
  case EColliderCollisionState::Stay:
    if (!m_is_collided_flag_setup)
      m_collision_state = EColliderCollisionState::Idle;
    break;
  }

  auto& obj = GetBindObject();

  std::string state_name;
  switch (m_collision_state) {
  case EColliderCollisionState::Idle:
    state_name = "Idle";
    break;
  case EColliderCollisionState::Enter:
    state_name = "Enter";
    break;
  case EColliderCollisionState::Stay:
    state_name = "Stay";
    break;
  }
  PUSH_LOG_WARN_EXT("{} state transits into {}.", obj.GetGameObjectName(), state_name);

  obj.pCallPhysicsCallback(m_collision_state,
                           is_collision_function,
                           other_collider);
}

bool CProtoRigidbodyCollider2D::pIsCallbackFunctionCalledOnThisFrame() const noexcept {
  return m_is_collided_flag_setup;
}

} /// ::opgs16::component namespace