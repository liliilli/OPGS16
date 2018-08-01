#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_static_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Internal/rigidbody_static_base.h>

namespace opgs16::component::_internal {

void CRigidbodyStaticBase::SetLinearLimitFactor(bool x, bool y, bool z) noexcept {
  m_limit_factor.x = x;
  m_limit_factor.y = x;
  m_limit_factor.z = x;

  auto& colliders = pGetColliderContainer();
  for (auto& [uid, collider] : colliders) {
    collider->pSetLinearFactor(m_limit_factor);
  }
}

void CRigidbodyStaticBase::SetLinearLimitFactor(const DLinearLimitFactor& factor) noexcept {
  m_limit_factor = factor;

  auto& colliders = pGetColliderContainer();
  for (auto& [uid, collider] : colliders) {
    collider->pSetLinearFactor(m_limit_factor);
  }
}

const DLinearLimitFactor& CRigidbodyStaticBase::GetLinearLimitFactor() const noexcept {
  return m_limit_factor;
}

} /// ::opgs16::component::_internal namesapce