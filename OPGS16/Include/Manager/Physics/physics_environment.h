#ifndef OPGS16_MANAGER_INTERNAL_PHYSICS_ENVIRONMENT_H
#define OPGS16_MANAGER_INTERNAL_PHYSICS_ENVIRONMENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/physics_environment.h
///
/// @brief
/// Declaration file of physics environment for processing physics.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-04 Create file.
/// 2018-06-06 Move file to OPGS16 project.
///

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

/// ::phitos::enums::EInitated
#include <Phitos/Enums/initiated.h>
/// ::opgs16::EAxis3D
#include <Helper/Type/axis.h>

namespace opgs16::manager::physics::_internal {

///
/// @enum EPhysicsEnvironment
/// @brief
/// Fixed environment value parameter for getting CPhysicsEnvironment generated
/// fixed physics environment.
///
enum class EPhysicsEnvironment {
  Default,  // Default environment, discrete, and ordinary.
};

///
/// @class CPhysicsEnvironment
///
/// @brief
/// Physics environment for processing physics calculation and ecosystem.
/// This class must be initiated with EPhysicsEnvironment parameter prior to
/// processing any collision, physics calculation, rigidbody and collision sha
/// pe creation.
///
/// @author Jongmin Yun
///
class CPhysicsEnvironment final {
public:
  //!
  //! Special functions.
  //!

  /// This default constructor will not create physics environment.
  /// Must use another constructor which receives EPhysicsEnvironment parameter.
  /// or etc.
  CPhysicsEnvironment() = default;
  /// Generate fixed physics environment with EPhysicsEnvironment paramter.
  CPhysicsEnvironment(EPhysicsEnvironment environment_style);

  ~CPhysicsEnvironment();

  CPhysicsEnvironment(const CPhysicsEnvironment&) = delete;
  CPhysicsEnvironment(CPhysicsEnvironment&&) = delete;
  CPhysicsEnvironment& operator=(const CPhysicsEnvironment&) = delete;
  CPhysicsEnvironment& operator=(CPhysicsEnvironment&&) = delete;

  //!
  //! Function declarations.
  //!

  ///
  /// @brief
  /// Set global gravity of environment, with separated x, y, z float value.
  /// gravity axis coordinate uses right-hand coordinate.
  /// If you apply for the earth gravity, just put -9.81f into y parameter except
  /// for x, and z are 0.f.
  ///
  /// @param[in] x x axis gravity value.
  /// @param[in] y y axis gravity value.
  /// @param[in] z z axis gravity value.
  ///
  void SetGlobalGravity(float x, float y, float z) noexcept;

  ///
  /// @brief
  /// Set global gravity of environment, with axis and gravity_value parameter.
  /// gravity axis coordinate uses right-hand coordinate.
  /// If you apply for the earth gravity, just put -9.81f into y parameter except
  /// for x, and z are 0.f.
  ///
  /// @param[in] axis Axis parameter for specifying one of three axis.
  /// @param[in] gravity_value Gravity value for being apply for to axis.
  ///
  void SetGlobalGravity(EAxis3D axis, float gravity_value) noexcept;

  ///
  /// @brief
  /// Add rigidbody to dynamic world.
  /// Added rigidbody must be removed prior to destroying object;
  /// otherwise rigidbody will be dangling pointer.
  ///
  /// Any third users have not to use this function manually, because of
  /// support of counting reference rigidbody object implementation which do
  /// this automatically.
  ///
  /// @param[in] rigidbody_rawptr The pointer address of any valid rigidbody.
  ///
  /// @warning When addition is failed, it might occur unexpected consequence.
  ///
  void AddRigidbody(btRigidBody* rigidbody_rawptr) noexcept;

  ///
  /// @brief
  /// Remove rigidbody with valid pointer address of rigidbody instance.
  /// Any thrid users have not to use this function manually, because of
  /// supporting of counting reference rigidbody object implementation which do
  /// this automatically.
  ///
  /// @param[in] rigidbody_rawptr The pointer address of any valid rigibody.
  ///
  /// @warning When remove is failed, it might occur unexpected consequence.
  ///
  void RemoveRigidbody(btRigidBody* rigidbody_rawptr) noexcept;

  ///
  /// @brief
  /// Update physics world.
  ///
  /// @param[in] time_delta Time interval of updating physics calculation.
  /// 1.0f is 1 second.
  ///
  void PhysicsUpdate(float time_delta);

private:
  ///
  /// @brief
  /// Debug mode function, check whether dynamic world is initiated or not.
  ///
  void DebugCheckWorldInitiated() const noexcept;

  using EInitiated = phitos::enums::EInitiated;

  btCollisionConfiguration* m_collision_configuration = nullptr;
  btCollisionDispatcher*    m_collision_dispatcher = nullptr;
  btBroadphaseInterface*    m_overlapping_pair_cache = nullptr;
  btConstraintSolver*       m_solver = nullptr;
  btDynamicsWorld*          m_dynamics_world = nullptr;

  mutable EInitiated m_is_initiated = EInitiated::NotInitiated;
};

} /// ::trial namespace

#endif /// TRIAL_PHYSICS_MANAGEMENT_H
