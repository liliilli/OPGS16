#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file
///

/// Header file
#include <Component/Internal/rigidbody_base.h>

namespace opgs16::component::_internal {

CRigidbodyBase::~CRigidbodyBase() {
  for (auto& [uid, collider] : m_collider_container) {
    collider->m_bind_rigidbody = nullptr;
  }
  m_collider_container.clear();
}

CColliderBase* CRigidbodyBase::pFindColliderInContainer(CColliderBase* collider) {
  const auto uid  = collider->GetUniqueIndexValue();
  const auto it   = m_collider_container.find(uid);
  if (it == m_collider_container.end())
    return nullptr;
  else
    return it->second;
}

CColliderBase* CRigidbodyBase::pSetColliderInContainer(CColliderBase* collider) {
  const auto uid  = collider->GetUniqueIndexValue();

  auto [it, result] = m_collider_container.try_emplace(uid, collider);
  if (result) {
    return it->second;
  }
  else {
    m_collider_container.erase(it);
    return nullptr;
  }
}

bool CRigidbodyBase::pUnbindColliderInContainer(CColliderBase* collider) {
  const auto uid = collider->GetUniqueIndexValue();

  const auto removed_size = m_collider_container.erase(uid);
  return removed_size > 0;
}

CRigidbodyBase::TColliderContainer& CRigidbodyBase::pGetColliderContainer() noexcept {
  return m_collider_container;
}

} /// ::opgs16::component::_internal namespace