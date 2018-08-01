#ifndef OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H
#define OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file
///

#include <Component/Internal/collider_base.h>
#include <Component/Internal/component.h>

namespace opgs16::component::_internal {

class CRigidbodyBase : public CComponent {
  using TColliderContainer = std::unordered_map<uint32_t, CColliderBase*>;

public:
  CRigidbodyBase(element::CObject& bind_object) : CComponent{ bind_object } {}

  ~CRigidbodyBase();

  CRigidbodyBase(const CRigidbodyBase&) = default;
  CRigidbodyBase(CRigidbodyBase&&)      = default;
  CRigidbodyBase& operator=(const CRigidbodyBase&) = default;
  CRigidbodyBase& operator=(CRigidbodyBase&&)      = default;

protected:
  CColliderBase* pFindColliderInContainer(CColliderBase* collider);

  CColliderBase* pSetColliderInContainer(CColliderBase* collider);

  TColliderContainer& pGetColliderContainer() noexcept;

  bool pUnbindColliderInContainer(CColliderBase* collider);

private:
  virtual void pEnrollColliderOnObject(CColliderBase* collider) = 0;

  TColliderContainer  m_collider_container;

  friend opgs16::component::_internal::CColliderBase;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRigidbodyBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H