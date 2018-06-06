#ifndef OPGS16_COMPONENT_PHYSICS_COLLIDER2D_BOX_H
#define OPGS16_COMPONENT_PHYSICS_COLLIDER2D_BOX_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/collider2d_box.h
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-06 Create file.
///
/// @bug DO NOT USE THIS COMPONENT UNTIL NEXT UPDATE!
///

/// ::opgs16::component::physics::CColliderGeneral
#include <Component/Physics/collider_general.h>

namespace opgs16::component::physics {

class CCollider2DBox final : public CColliderGeneral {
SET_UP_TYPE_MEMBER(::opgs16::component::physics::CColliderGeneral, CCollider2DBox)
public:
  CCollider2DBox(element::CObject& bind_object,
                 const std::string& shape_name, const DVector2& half_size);

private:
};

} /// ::opgs16::component::physics namespace

#endif /// OPGS16_COMPONENT_PHYSICS_COLLIDER2D_BOX_H