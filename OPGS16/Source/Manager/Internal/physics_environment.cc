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

/// ::phitos:: enhanced assertion.
#include <Phitos/Dbg/assert.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

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
  DebugCheckWorldInitiated();

  m_dynamics_world->stepSimulation(delta_time, 10);

  for (auto j = m_dynamics_world->getNumCollisionObjects() - 1; j >= 0; --j) {
    auto collision_obj = m_dynamics_world->getCollisionObjectArray()[j];
    auto rigidbody_obj = btRigidBody::upcast(collision_obj);

    btTransform trans;
    if (rigidbody_obj && rigidbody_obj->getMotionState()) {
      rigidbody_obj->getMotionState()->getWorldTransform(trans);
    }
    else {
      trans = rigidbody_obj->getWorldTransform();
    }

    printf("World pos object %d = %f, %f, %f\n",
      j,
      float(trans.getOrigin().getX()),
      float(trans.getOrigin().getY()),
      float(trans.getOrigin().getZ())
    );
  }
}

void CPhysicsEnvironment::DebugCheckWorldInitiated() const noexcept {
  PHITOS_ASSERT(m_is_initiated == EInitiated::Initiated,
      "Physics environment is still not initiated.");
  PHITOS_ASSERT(m_dynamics_world != nullptr,
      "Physics dynamic worlds which calculate physics objects are not initiated.");
}

} /// ::opgs16::manager::physics::_internal namespace