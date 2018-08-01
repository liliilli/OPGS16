#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/rigidbody2d_static.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Physics/rigidbody2d_static.h>
#include <Component/Internal/collider2d_base.h>
#include <Headers/import_logger.h>

namespace opgs16::component {

void CRigidbodyStatic2D::InitiateColliders() {
  Update(0);
}

void CRigidbodyStatic2D::Update(float delta_time) {
  if (m_is_dirty) {

    // Instantiate collider object resources and destroy m_candidate.
    for (auto& collider : m_candidates) {
      auto result_collider = pSetColliderInContainer(collider);
      if (result_collider) {
        result_collider->pSetColliderType(EColliderActualType::Staic);
        result_collider->pSetMass(0);
        result_collider->pSetLinearFactor(GetLinearLimitFactor());
        result_collider->pInitializeCollider();
      }
    }

    m_candidates.clear();
    m_is_dirty = false;
  }
}

void CRigidbodyStatic2D::pEnrollColliderOnObject(_internal::CColliderBase* collider) {
  // @todo need to implement UE4's cast function.
  auto instance = static_cast<_internal::CCollider2DBase*>(collider);

  if (pFindColliderInContainer(instance)) {
    PUSH_LOG_WARN_EXT("{} is already in CRigidbody container.",
                       collider->GetUniqueIndexValue());
    return;
  }

  m_candidates.push_back(instance);
  m_is_dirty = true;
}

} /// ::opgs16::component namespace