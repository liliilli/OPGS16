#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/collider2d_box.cc
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

/// Header file
#include <Component/Physics/collider2d_box.h>

namespace opgs16::component::physics {

CCollider2DBox::CCollider2DBox(element::CObject& bind_object,
    const std::string& shape_name, const DVector2& half_size) :
    CColliderGeneral(bind_object) {
  // Body
  AddCollisionShapeWithRigidbody<EColliderType::Box2D>(
      shape_name.c_str(),
      static_cast<btVector3>(half_size));
  BindRigidbodyToDynamicWorld();
  SetLinearFactor(EAxis3D::Z, Switch::ON);

  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

} /// ::opgs16::component::physics namespace