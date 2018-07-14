
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_cup_item.h"

#include "../../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_ball.h"

#include "../../../../Include/Script/SceneGamePlay/Cup/script_item_movement.h"

namespace magiccup {

ObjectCupItem::ObjectCupItem() {
  Instantiate<ObjectBall>(ObjectBall::s_object_name);
  auto cup = Instantiate<ObjectCup>(ObjectCup::s_object_name);
  cup->SetWorldPosition({0.f, 48.f, 0.f});

  AddComponent<ScriptItemMovement>(*this);
}

} /// ::magiccup namespace