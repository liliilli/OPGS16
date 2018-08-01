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

CColliderBox2D::CColliderBox2D(
    element::CObject& bind_object,
    const DVector2& collider_size) : CCollider2DBase{ bind_object } {
  m_collider_size = collider_size;
}

void CColliderBox2D::pInitializeCollider() {
  auto& obj = GetBindObject();
  m_bind_info.bind_object   = &obj;
  m_bind_info.bind_collider = this;

  auto rigidbody = GetLocalRigidbody();
  pCreatebtRigidbody(pGetMass(), rigidbody);
  __pUpdateFlags();
  //if (is_kinematic) {
    //(*rigidbody)->setActivationState(DISABLE_DEACTIVATION);
  //}

  opgs16::manager::physics::AddRigidbody(*rigidbody);

  //pSetCollisionActualType(EColliderActualType::Dynamic);
  pSetCollisionState(EColliderBehaviorState::Activated);

  // Create CPrivateAabbRenderer2D and set information.
  m_aabb_renderer = std::make_unique<_internal::CPrivateAabbRenderer2D>(obj, this);
  m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(m_collider_size));
  m_aabb_renderer->SetCollisionRenderPosition(obj.GetFinalPosition());
}

void CColliderBox2D::pCreatebtRigidbody(
    const float mass_sum,
    btRigidBody** rigidbody_ptr) {
  auto& obj = GetBindObject();

  // Create collision shape
  const DVector2 half_size = m_collider_size * 0.5f;
  auto shape = GetCollisionShape();
  (*shape) = new btBox2dShape{{half_size.x, half_size.y, 0.f}};
  (*shape)->setMargin(1.f);

  // Create motion state
  btQuaternion rotation;
  rotation.setEulerZYX(obj.GetRotationWpAngle(EAxis3D::Z),
                       obj.GetRotationWpAngle(EAxis3D::Y),
                       obj.GetRotationWpAngle(EAxis3D::X));
  btDefaultMotionState* motionState =
      new btDefaultMotionState(btTransform{rotation, obj.GetFinalPosition()});


  // Create rigidbody info
  btVector3 local_inertia;
  //m_collision_shape->calculateLocalInertia(mass_sum, local_inertia);
  auto body_construction_info = btRigidBody::btRigidBodyConstructionInfo{
      (mass_sum <= 0.f) ? 0.001f : mass_sum,
      motionState,
      *shape,
      local_inertia};
  body_construction_info.m_restitution = 0.0f;
  body_construction_info.m_friction = 1.0f;

  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setUserPointer(static_cast<void*>(&m_bind_info));

  // Restrict physical influence to (x, y) axis only.
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});
}

void CColliderBox2D::Update(float delta_time) {
  auto& obj = GetBindObject();
  const auto& position = obj.GetFinalPosition();
  auto rigidbody = *GetLocalRigidbody();

  if (!m_is_position_initialized) {
    btTransform transform;
    rigidbody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(position);

    rigidbody->setWorldTransform(transform);
    rigidbody->getMotionState()->setWorldTransform(transform);
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

  if (!rigidbody->isActive() && GetColliderType() != EColliderActualType::Staic) {
    pSetCollisionState(EColliderBehaviorState::Sleep);
  }
}

#ifdef false
void CColliderBox2D::SetMass(float mass_value) {
  if (mass_value <= 0) {
    mass_value = 0.001f;
  }

  //btVector3 local_inertia;
  //m_collision_shape->calculateLocalInertia(mass_value, local_inertia);
  auto rigidbody = GetLocalRigidbody();
  (*rigidbody)->setMassProps(mass_value, {0, 0, 0});
}

void ccolliderbox2d::setkinematic(bool is_kinematic) {
  auto rigidbody = getlocalrigidbody();

  if (is_kinematic) {
    (*rigidbody)->setcollisionflags(
        (*rigidbody)->getcollisionflags() | btcollisionobject::cf_kinematic_object);
    m_collider_type = ecollideractualtype::kinetic;
  }
  else {
    (*rigidbody)->setcollisionflags(
        (*rigidbody)->getcollisionflags() | btcollisionobject::cf_character_object);
    m_collider_type = ecollideractualtype::dynamic;
  }
}
#endif

void CColliderBox2D::TemporalSetStatic() {
  auto rigidbody = GetLocalRigidbody();

  //(*rigidbody)->setMassProps(0, {});
  //m_collider_type = EColliderActualType::Staic;
}

void CColliderBox2D::SetColliderSize(const DVector2& size) {
  auto shape = GetCollisionShape();
  auto rigidbody = GetLocalRigidbody();

  if (*shape)
    delete *shape;

  (*shape) = new btBox2dShape{
      static_cast<btVector3>(size * 0.5f)
  };
  (*rigidbody)->setCollisionShape((*shape));
  m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(size));
}

CColliderBox2D::EColliderBehaviorState
CColliderBox2D::GetColliderState() const noexcept {
  return m_state;
}

void CColliderBox2D::pUpdateAabbToRenderer(const DVector3& min,
                                                      const DVector3& max) {
  if (m_aabb_renderer) {
    m_aabb_renderer->SetCollisionSize(max - min);
    m_aabb_renderer->SetCollisionRenderPosition((max + min) / 2);
    m_aabb_renderer->Update(0);
    opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
  }
}

void CColliderBox2D::pSetCollisionState(EColliderBehaviorState state) {
  PHITOS_ASSERT(state != EColliderBehaviorState::None,
                "Collision state must not be None.");

  if (state == EColliderBehaviorState::Collided) {
    m_is_collided_flag_setup = true;
  }

  m_state = state;
}

void CColliderBox2D::pCallBindObjectCallback(CColliderBox2D* other_collider) {
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

bool CColliderBox2D::pIsCallbackFunctionCalledOnThisFrame() const noexcept {
  return m_is_collided_flag_setup;
}

} /// ::opgs16::component namespace