
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

#include "../../Include/Object/SceneMain/ui_title_part.h"
#include "../../Include/Object/SceneMain/ui_scoreboard_part.h"
#include "../../Include/Script/SceneMain/script_transition_manage.h"
#include "../../Include/Object/SceneMain/ui_background_part.h"

namespace magiccup {

void SceneMain::Initiate() {
  SetBackgroundColor(opgs16::DColor::Black);

  auto canvas = CreateGameObject<magiccup::EmptyCanvas>(name::canvas);
  canvas->CreateGameObject<UiBackgroundPart>(UiBackgroundPart::s_object_name);
  canvas->CreateGameObject<UiTitlePart>(UiTitlePart::s_object_name);
  canvas->CreateGameObject<UiScoreboardPart>(UiScoreboardPart::s_object_name);
  canvas->AddComponent<ScriptTransitionManagement>(*canvas);
}

} /// ::magiccup namespace
