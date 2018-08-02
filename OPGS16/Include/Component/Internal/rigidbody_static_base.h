#ifndef OPGS16_COMPONENT_INTERNAL_RIGIDBODY_STATIC_BASE_H
#define OPGS16_COMPONENT_INTERNAL_RIGIDBODY_STATIC_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_static_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

#include <Component/Internal/rigidbody_base.h>

namespace opgs16::component::_internal {

///
/// @class CRigidbodyStaticBase
/// @brief Base type of all static rigidbody types.
///
class CRigidbodyStaticBase : public CRigidbodyBase {
  using EColliderActualType = element::_internal::EColliderActualType;

public:
  CRigidbodyStaticBase(element::CObject& bind_object) : CRigidbodyBase { bind_object } {};

  const DLinearLimitFactor& GetLinearLimitFactor() const noexcept;

  ///
  /// @brief Set linear limit factors and propagtes it all binded colliders.
  ///
  void SetLinearLimitFactor(bool x, bool y, bool z) noexcept;

  void SetLinearLimitFactor(const DLinearLimitFactor& factor) noexcept;

private:
  DLinearLimitFactor m_limit_factor   = {true, true, true};
  EColliderActualType m_collider_type = EColliderActualType::Static;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRigidbodyBase, CRigidbodyStaticBase)
};

}

#endif /// OPGS16_COMPONENT_INTERNAL_RIGIDBODY_STATIC_BASE_H