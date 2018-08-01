#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_dynamic_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Internal/rigidbody_dynamic_base.h>
#include <Manager/physics_manager.h>
#include <Manager/Physics/physics_environment.h>

namespace opgs16::component::_internal {

float CRigidbodyDynamicBase::GetMass() const noexcept {
  return m_mass_sum;
}

bool CRigidbodyDynamicBase::IsUseGravity() const noexcept {
  return m_is_use_gravity;
}

bool CRigidbodyDynamicBase::IsKinematic() const noexcept {
  return m_collider_type == EColliderActualType::Staic;
}

void CRigidbodyDynamicBase::SetMass(float mass_sum) noexcept {
  if (mass_sum < 0.001f)
    mass_sum = 0.001f;

  m_mass_sum = mass_sum;

  // Propagate all collider.
}

void CRigidbodyDynamicBase::SetKinematic(bool is_kinematic) noexcept {
  auto& colliders = pGetColliderContainer();

  if (is_kinematic) {
    for (auto& [uid, collider] : colliders) {
      if (!collider) continue;

      auto rigidbody = *collider->GetLocalRigidbody();
      if (!rigidbody) continue;

      rigidbody->setCollisionFlags(
          rigidbody->getCollisionFlags() |
          btCollisionObject::CF_KINEMATIC_OBJECT
      );

      m_collider_type = EColliderActualType::Kinetic;
    }
  }
  else {
    for (auto& [uid, collider] : colliders) {
      if (!collider) continue;

      auto rigidbody = *collider->GetLocalRigidbody();
      if (!rigidbody) continue;

      rigidbody->setCollisionFlags(
          rigidbody->getCollisionFlags() &
          ~btCollisionObject::CF_KINEMATIC_OBJECT
      );

      m_collider_type = EColliderActualType::Dynamic;
    }
  }
}

void CRigidbodyDynamicBase::SetUseGravity(bool using_gravity) noexcept {
  using opgs16::manager::physics::GetManagement;

  if (using_gravity) {
    const auto gravity = GetManagement()->GetGlobalGravity();

    // Propagate all colllider.
  }
  else {

    // Propagate all collider.
  }
}

void CRigidbodyDynamicBase::SetLinearLimitFactor(bool x, bool y, bool z) noexcept {
  m_limit_factor.x = x;
  m_limit_factor.y = x;
  m_limit_factor.z = x;

  // Propagate all collider.
}

void CRigidbodyDynamicBase::SetLinearLimitFactor(const DLinearLimitFactor& factor) noexcept {
  m_limit_factor = factor;

  // Propagate all collider.
}

const DLinearLimitFactor& CRigidbodyDynamicBase::GetLinearLimitFactor() const noexcept {
  return m_limit_factor;
}

} /// ::opgs16::component::_internal namespace