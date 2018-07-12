
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/ObjectTest/node_canvas_script.h"

#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Internal/keyword.h"
#include "../../../Include/Scene/ObjectTest/object_test_select.h"

namespace debug::script {

void NodeCanvasScript::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;

  if (IsKeyPressed(keyword::key_backspace)) {
    M_REPLACE_SCENE(scene::ObjectTestSelectScene);
  }
}

} /// ::debug::script namespace