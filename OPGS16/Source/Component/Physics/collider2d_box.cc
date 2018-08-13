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

#include <Element/object.h>
#include <Manager/physics_manager.h>

namespace opgs16::component {

CColliderBox2D::CColliderBox2D(element::CObject& bind_object,
                               const DVector2& collider_size) :
                               CCollider2DBase{ bind_object } {
  m_collider_size = collider_size;
}

void CColliderBox2D::pInitializeCollider() {
  auto rigidbody = GetLocalRigidbody();
  pCreatebtRigidbody(pGetMass(), rigidbody);
  pUpdateColliderTypeFlag();
#ifdef false
  if (is_kinematic) {
    (*rigidbody)->setActivationState(DISABLE_DEACTIVATION);
  }
#endif

  opgs16::manager::physics::AddRigidbody(*rigidbody);
  pfSetBehaviorState(EColliderBehaviorState::Activated);
}

void CColliderBox2D::pCreatebtRigidbody(float mass_sum, btRigidBody** rigidbody_ptr) {
  // Create collision shape
  const DVector2 half_size = m_collider_size * 0.5f;
  auto shape = GetCollisionShape();
  (*shape) = new btBox2dShape{{half_size.x, half_size.y, 0.f}};
  (*shape)->setMargin(4.f);

  // Create motion state
  auto& obj = GetBindObject();
  btQuaternion rotation;
  rotation.setEulerZYX(obj.GetFinalRotationAngle(EAxis3D::Z),
                       obj.GetFinalRotationAngle(EAxis3D::Y),
                       obj.GetFinalRotationAngle(EAxis3D::X));
  auto* motionState = new btDefaultMotionState(btTransform{rotation, obj.GetFinalPosition()});

  // Create rigidbody info
  auto body_construction_info = btRigidBody::btRigidBodyConstructionInfo{
      mass_sum, motionState, *shape, btVector3{}};
  body_construction_info.m_restitution = 0.0f;
  body_construction_info.m_friction    = 5.0f;

  // Restrict physical influence to (x, y) axis only.
  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});
  pSetColliderUserPointerAndBind();
  pInitiateAabbRenderer(true);
}

void CColliderBox2D::Update(float delta_time) {
  auto rigidbody = *GetLocalRigidbody();
  if (!rigidbody) return;
  CColliderBase::Update(0);

  // First contact, Reset collider position to CObject's final position.
  if (!m_is_position_initialized) {
    auto motion_state = rigidbody->getMotionState();

    btTransform transform;
    motion_state->getWorldTransform(transform);
    transform.setOrigin(GetBindObject().GetFinalPosition());

    rigidbody->setWorldTransform(transform);
    motion_state->setWorldTransform(transform);
    m_is_position_initialized = true;
  }

  if (!rigidbody->isActive() && GetColliderType() != EColliderActualType::Static) {
    pfSetBehaviorState(EColliderBehaviorState::Sleep);
  }
}

void CColliderBox2D::SetColliderSize(const DVector2& size) {
  m_collider_size = size;

  if (GetBehaviorState() != EColliderBehaviorState::None) {
    auto shape = GetCollisionShape();
    auto rigidbody = GetLocalRigidbody();

    if (*shape) delete *shape;
    (*shape) = new btBox2dShape{ static_cast<btVector3>(size * 0.5f) };
    (*rigidbody)->setCollisionShape((*shape));
  }
}

} /// ::opgs16::component namespace