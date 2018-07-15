
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

namespace magiccup {

void SceneGamePlay::Initiate() {
  SetBackgroundColor(opgs16::DColor::DarkGray);

  auto canvas = CreateGameObject<EmptyCanvas>(name::canvas);
  canvas->CreateGameObject<magiccup::GamePlayUi>(GamePlayUi::s_obj_name);

  auto camera = CreateGameObject<OrthogonalCameraObject>(
      OrthogonalCameraObject::s_object_name);

  auto management = CreateGameObject<StageObjectManagement>(
      StageObjectManagement::s_object_name);
  management->SetWorldPosition({128.f, 80.f, 0.f});

  auto total_management = CreateGameObject<TotalManagement>(TotalManagement::s_object_name);
}

} /// ::magiccup namespace