#ifndef OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_H
#define OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/rigidbody2d.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

#include <Component/Internal/rigidbody_dynamic_base.h>
#include "Component/Internal/collider2d_base.h"

namespace opgs16::component {

class CRigidbody2D final : public _internal::CRigidbodyDynamicBase {
public:
  CRigidbody2D(element::CObject& bind_object) : CRigidbodyDynamicBase{ bind_object } { }

private:
  void pEnrollColliderOnObject(_internal::CColliderBase* collider) override final;

  void Update(float delta_time) override final;

  std::vector<_internal::CCollider2DBase*> m_candidates;
  bool m_is_dirty = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRigidbodyDynamicBase, CRigidbody2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_H