
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Disposable/effect_obj_gameover.h"

#include <Element/object.h>
#include <Manager/scene_manager.h>

#include "../../../../Include/Internal/general_keyword.h"
#include "../../../../Include/Internal/object_keyword.h"
#include "../../../../Include/Object/SceneGamePlay/object_cursor.h"
#include "../../../../Include/Object/SceneGamePlay/effect_boom_object.h"

namespace magiccup {

void EffectObjectGameOver::Initiate() {
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();
  auto destruction_point = obj.GetGameObject(ObjectCursor::s_object_name);

  destruction_point->SetObjectActive(false);
  const auto point = destruction_point->GetFinalPosition();

  auto bomb = GetPresentScene()->CreateGameObject<EffectBoomObject>("Boom");
  bomb->SetWorldPosition(point);
}

void EffectObjectGameOver::Update(float delta_time) {
  GetBindObject().RemoveComponent<EffectObjectGameOver>();
}

} /// ::magiccup namespace