#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody2d.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Physics/rigidbody2d.h>
#include <Component/Internal/collider2d_base.h>
#include <Headers/import_logger.h>

namespace opgs16::component {

void CRigidbody2D::pEnrollColliderOnObject(_internal::CColliderBase* collider_instance) {
  // @todo need to implement UE4's cast function.
  auto collider = static_cast<_internal::CCollider2DBase*>(collider_instance);
  if (pFindColliderInContainer(collider)) {
    PUSH_LOG_WARN_EXT("{} is already in CRigidbody container.",
                       collider->GetUniqueIndexValue());
    return;
  }

  m_candidates.push_back(collider);
  m_is_dirty = true;
  SetComponentActive(true);
}

void CRigidbody2D::InitiateColliders() {
  Update(0);
}

void CRigidbody2D::Update(float delta_time) {
  if (m_is_dirty) {
    using opgs16::element::_internal::EColliderBehaviorState;

    // Instantiate collider object resources and destroy m_candidate.
    for (auto& collider : m_candidates) {
      auto result_collider = pSetColliderInContainer(collider);
      if (result_collider) {
        result_collider->pSetMass(GetMass());
        result_collider->pSetColliderType(
            IsKinematic() ?
            EColliderActualType::Kinetic :
            EColliderActualType::Dynamic);
        result_collider->pSetLinearFactor(GetLinearLimitFactor());
        result_collider->pInitializeCollider();
      }
    }

    m_candidates.clear();
    m_is_dirty = false;
  }
  SetComponentActive(false);
}

} /// ::opgs16::component namespace