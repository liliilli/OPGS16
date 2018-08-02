#ifndef OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_STATIC_H
#define OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_STATIC_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/rigidbody2d_static.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Internal/rigidbody_static_base.h>

//!
//! Forward declaration
//!

namespace opgs16::component::_internal {
class CCollider2DBase;
}

//!
//! Implementation
//!

namespace opgs16::component {

///
/// @class CRigidbodyStatic2D
/// @brief Only static 2d only rigidbody type.
///
class CRigidbodyStatic2D final : public _internal::CRigidbodyStaticBase {
public:
  CRigidbodyStatic2D(element::CObject& bind_object) : CRigidbodyStaticBase{ bind_object } {};

  ///
  /// @brief Manually initiate colliders to rigidbody.
  /// If you want to manipulate properties stimutaneously, use this.
  ///
  void InitiateColliders();

private:
  ///
  /// Just initiate not yet born colliders.
  ///
  void Update(float delta_time) override final;

  void pEnrollColliderOnObject(_internal::CColliderBase* collider) override final;

  /// Disposable not yet born collider candidates.
  std::vector<_internal::CCollider2DBase*> m_candidates;
  bool m_is_dirty = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRigidbodyStaticBase, CRigidbodyStatic2D)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PHYSICS_RIGIDBODY_2D_STATIC_H