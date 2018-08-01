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

#include <unordered_map>

#include <Component/Internal/collider_base.h>
#include <Component/Internal/component.h>

namespace opgs16::component::_internal {

class CRigidbodyStaticBase : public CComponent {
  using TColliderContainer = std::unordered_map<uint32_t, CColliderBase*>;

public:
  CRigidbodyStaticBase(element::CObject& bind_object) : CComponent { bind_object } {};

protected:
  CColliderBase* pFindColliderInContainer(CColliderBase* collider);

  CColliderBase* pSetColliderInContainer(CColliderBase* collider);

private:
  TColliderContainer  m_collider_container;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRigidbodyStaticBase)
};

}

#endif /// OPGS16_COMPONENT_INTERNAL_RIGIDBODY_STATIC_BASE_H