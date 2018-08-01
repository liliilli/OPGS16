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
#include <Component/Physics/collider2d_box.h>

#include <BulletCollision/CollisionShapes/btBox2dShape.h>

#include <Core/core_setting.h>
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
#ifdef false
  if (is_kinematic) {
    (*rigidbody)->setActivationState(DISABLE_DEACTIVATION);
  }
#endif

  opgs16::manager::physics::AddRigidbody(*rigidbody);
  pfSetBehaviorState(EColliderBehaviorState::Activated);
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

#ifdef false
  btVector3 local_inertia;
  m_collision_shape->calculateLocalInertia(mass_sum, local_inertia);
#endif

  // Create rigidbody info
  auto body_construction_info = btRigidBody::btRigidBodyConstructionInfo{
      mass_sum, motionState, *shape, btVector3{}};
  body_construction_info.m_restitution = 0.8f;
  body_construction_info.m_friction    = 1.0f;

  // Restrict physical influence to (x, y) axis only.
  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setUserPointer(static_cast<void*>(&m_bind_info));
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});

  pInitiateAabbRenderer(true);
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

  CColliderBase::Update(0);

  if (!rigidbody->isActive() &&
      GetColliderType() != EColliderActualType::Staic) {
    pfSetBehaviorState(EColliderBehaviorState::Sleep);
  }
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
  //m_aabb_renderer->SetCollisionSize(static_cast<DVector3>(size));
}

} /// ::opgs16::component namespace