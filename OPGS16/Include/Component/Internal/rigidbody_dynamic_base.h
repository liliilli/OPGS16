#ifndef OPGS16_COMPONENT_INTERNAL_RIGIDBODY_DYNAMIC_BASE_H
#define OPGS16_COMPONENT_INTERNAL_RIGIDBODY_DYNAMIC_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_dynamic_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

#include <Component/Internal/rigidbody_base.h>

namespace opgs16::component::_internal {

class CRigidbodyDynamicBase : public CRigidbodyBase {
  using EColliderActualType = element::_internal::EColliderActualType;

public:
  CRigidbodyDynamicBase(element::CObject& bind_object) : CRigidbodyBase { bind_object } {};

  float GetMass() const noexcept;

  bool IsUseGravity() const noexcept;

  bool IsKinematic() const noexcept;

  void SetMass(float mass_sum) noexcept;

  void SetKinematic(bool is_kinematic) noexcept;

  void SetUseGravity(bool using_gravity) noexcept;

  void SetLinearLimitFactor(bool x, bool y, bool z) noexcept;

  void SetLinearLimitFactor(const DLinearLimitFactor& factor) noexcept;

  const DLinearLimitFactor& GetLinearLimitFactor() const noexcept;

private:
  float m_mass_sum = 0.001f;
  DLinearLimitFactor m_limit_factor = {true, true, true};

  bool  m_is_use_gravity  = true;
  EColliderActualType m_collider_type = EColliderActualType::None;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRigidbodyBase, CRigidbodyDynamicBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_RIGIDBODY_DYNAMIC_BASE_H