
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../Include/Scene/scene_gameplay.h"

#include "../../Include/Internal/object_keyword.h"

#include "../../Include/Object/Common/empty_canvas.h"
#include "../../Include/Object/Common/orthogonal_camera.h"
#include "../../Include/Object/SceneGamePlay/stage_obj_mng.h"
#include "../../Include/Object/SceneGamePlay/ui_object.h"
#include "../../Include/Object/SceneGamePlay/total_management.h"
#include "../../Include/Object/Common/empty_object.h"
#include "../../Include/Script/SceneGamePlay/Management/script_sound_manage.h"

namespace magiccup {

void SceneGamePlay::Initiate() {
  auto sound_object = CreateGameObject<EmptyObject>("Sound");
  sound_object->AddComponent<ScriptSoundManage>(*sound_object);

  SetBackgroundColor(opgs16::DColor::DarkGray);

  auto canvas = CreateGameObject<EmptyCanvas>(name::canvas);
  canvas->CreateGameObject<magiccup::GamePlayUi>(GamePlayUi::s_obj_name);

  CreateGameObject<OrthogonalCameraObject>(OrthogonalCameraObject::s_object_name);

  auto management = CreateGameObject<StageObjectManagement>(
      StageObjectManagement::s_object_name);
  management->SetWorldPosition({128.f, 80.f, 0.f});

  CreateGameObject<TotalManagement>(TotalManagement::s_object_name);
}

} /// ::magiccup namespace