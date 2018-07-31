#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/physics_management.cc
///
/// @brief
/// Definition file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-04 Create file.
///

/// Header file
#include <Manager/Physics/physics_environment.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

/// ::phitos:: enhanced assertion.
#include <Phitos/Dbg/assert.h>
#include <Component/Physics/prot_rigidbody_collider2d.h>
#include <Element/object.h>
#include <Element/Internal/physics_collider_bind_info.h>

#define OP_CAST(__MAType__, __MAObject__) static_cast<__MAType__>(__MAObject__)

namespace opgs16::manager::physics::_internal {

CPhysicsEnvironment::CPhysicsEnvironment(EPhysicsEnvironment environment_style) {
  /// @todo Rewrite assertion message.
  PHITOS_ASSERT(m_is_initiated == EInitiated::NotInitiated,
      "Duplicated environment initialization is prohibited."
      "If you want to rearrange physics environment, call new function.");

  switch (environment_style) {
  case EPhysicsEnvironment::Default: {}
    m_collision_configuration = new btDefaultCollisionConfiguration();
    m_collision_dispatcher    = new btCollisionDispatcher(m_collision_configuration);
    m_overlapping_pair_cache  = new btDbvtBroadphase();
    m_solver                  = new btSequentialImpulseConstraintSolver();

    m_dynamics_world = new btDiscreteDynamicsWorld(
      m_collision_dispatcher,
      m_overlapping_pair_cache,
      m_solver,
      m_collision_configuration
    );

    m_is_initiated = EInitiated::Initiated;
    SetGlobalGravity(0, -98.0f, 0);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

CPhysicsEnvironment::~CPhysicsEnvironment() {
  delete m_dynamics_world;
  delete m_solver;
  delete m_overlapping_pair_cache;
  delete m_collision_dispatcher;
  delete m_collision_configuration;
}

void CPhysicsEnvironment::SetGlobalGravity(float x, float y, float z) noexcept {
  DebugCheckWorldInitiated();
  m_dynamics_world->setGravity({x, y, z});
}

void CPhysicsEnvironment::SetGlobalGravity(EAxis3D axis,
                                           float gravity_value) noexcept {
  DebugCheckWorldInitiated();

  const auto previous_gravity = m_dynamics_world->getGravity();
  switch (axis) {
  case EAxis3D::X:
    m_dynamics_world->setGravity(
      {gravity_value, previous_gravity.y(), previous_gravity.z()}
    );
    break;
  case EAxis3D::Y:
    m_dynamics_world->setGravity(
      {previous_gravity.x(), gravity_value, previous_gravity.z()}
    );
    break;
  case EAxis3D::Z:
    m_dynamics_world->setGravity(
      {previous_gravity.x(), previous_gravity.y(), gravity_value}
    );
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

void CPhysicsEnvironment::AddRigidbody(btRigidBody* rigidbody_rawptr) noexcept {
  DebugCheckWorldInitiated();

  m_dynamics_world->addRigidBody(rigidbody_rawptr);
}

void CPhysicsEnvironment::RemoveRigidbody(btRigidBody* rigidbody_rawptr) noexcept {
  DebugCheckWorldInitiated();

  m_dynamics_world->removeRigidBody(rigidbody_rawptr);
}

void CPhysicsEnvironment::PhysicsUpdate(float delta_time) {
  using opgs16::element::CObject;
  using opgs16::component::CProtoRigidbodyCollider2D;

  DebugCheckWorldInitiated();

  m_dynamics_world->stepSimulation(delta_time, 5);

  pUpdatePostProcessRigidbodyInformation();
  pCallCollidedObjectCallbacks();
}

void CPhysicsEnvironment::pUpdatePostProcessRigidbodyInformation() {
  using opgs16::element::_internal::DPrivateColliderBindInfo;

  for (auto j = m_dynamics_world->getNumCollisionObjects() - 1; j >= 0; --j) {
    const auto collision_obj = m_dynamics_world->getCollisionObjectArray()[j];
    auto rigidbody_obj = btRigidBody::upcast(collision_obj);

    if (rigidbody_obj && rigidbody_obj->getMotionState()) {
      btTransform trans;
      rigidbody_obj->getMotionState()->getWorldTransform(trans);

      // Update position and aabb information.
      auto obj_ptr = static_cast<DPrivateColliderBindInfo*>(rigidbody_obj->getUserPointer());
      obj_ptr->bind_object->SetWorldPosWithFinalPos(trans.getOrigin());

      btVector3 min, max;
      rigidbody_obj->getAabb(min, max);
      obj_ptr->bind_collider->pUpdateAabbToRenderer(min, max);
    }
  }
}

void CPhysicsEnvironment::pCallCollidedObjectCallbacks() const noexcept {
  using opgs16::element::_internal::DPrivateColliderBindInfo;

  auto* dispatcher = m_dynamics_world->getDispatcher();
  const int32_t num_manifolds = dispatcher->getNumManifolds();
  for (int32_t i = 0; i < num_manifolds; ++i) {
    auto* contact_manifold = dispatcher->getManifoldByIndexInternal(i);
    const auto* obj_a = contact_manifold->getBody0();
    const auto* obj_b = contact_manifold->getBody1();

    const int32_t num_contacts = contact_manifold->getNumContacts();
    for (int32_t j = 0; j < num_contacts; ++j) {
      auto& pt = contact_manifold->getContactPoint(j);
      if (pt.getDistance() < 0.f) {
        const auto a_bind_ptr = OP_CAST(DPrivateColliderBindInfo*, obj_a->getUserPointer());
        const auto b_bind_ptr = OP_CAST(DPrivateColliderBindInfo*, obj_b->getUserPointer());

        if (!a_bind_ptr || !b_bind_ptr) {
          PUSH_LOG_CRITICAL("Data which binded to collider must be DPrivateColliderBindInfo.");
          PHITOS_UNEXPECTED_BRANCH();
        }

        auto* a_collider = a_bind_ptr->bind_collider;
        auto* b_collider = b_bind_ptr->bind_collider;

        PUSH_LOG_CRITICAL_EXT("Object Collided. A : {}, B : {}",
            a_bind_ptr->bind_object->GetGameObjectName(),
            b_bind_ptr->bind_object->GetGameObjectName());

        using opgs16::element::_internal::EColliderBehaviorState;
        a_collider->pSetCollisionState(EColliderBehaviorState::Collided);
        b_collider->pSetCollisionState(EColliderBehaviorState::Collided);

        a_collider->pCallBindObjectCallback(b_bind_ptr->bind_collider);
        b_collider->pCallBindObjectCallback(a_bind_ptr->bind_collider);
      }
    }
  }
}

void CPhysicsEnvironment::DebugCheckWorldInitiated() const noexcept {
  PHITOS_ASSERT(m_is_initiated == EInitiated::Initiated,
      "Physics environment is still not initiated.");
  PHITOS_ASSERT(m_dynamics_world != nullptr,
      "Physics dynamic worlds which calculate physics objects are not initiated.");
}

} /// ::opgs16::manager::physics::_internal namespace