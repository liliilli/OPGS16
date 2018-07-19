
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../Include/Scene/scene_main.h"

#include "../../Include/Internal/object_keyword.h"
#include "../../Include/Object/Common/empty_canvas.h"

#include "../../Include/Script/SceneMain/script_component.h"
#include "../../Include/Script/SceneMain/script_select.h"
#include "../../Include/Script/SceneMain/script_title.h"

namespace magiccup {

void SceneMain::Initiate() {
  SetBackgroundColor(opgs16::DColor::Black);

  auto canvas = CreateGameObject<magiccup::EmptyCanvas>(name::canvas);
  canvas->AddComponent<ScriptTitleSelect>(*canvas);
  canvas->AddComponent<ScriptTitleDisplay>(*canvas);
  canvas->AddComponent<ScriptTitleComponent>(*canvas);
}

} /// ::magiccup namespace
