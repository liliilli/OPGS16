#ifndef OPGS16_COMPONENT_INTERNAL_COLLIDER3D_H
#define OPGS16_COMPONENT_INTERNAL_COLLIDER3D_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/collider3d.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

#include <Component/Internal/collider_base.h>

namespace opgs16::component::_internal {

class CCollider3DBase : public CColliderBase {
public:
  CCollider3DBase(opgs16::element::CObject& bind_object) :
      CColliderBase{ bind_object } {};

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CColliderBase, CCollider3DBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_COLLIDER3D_H