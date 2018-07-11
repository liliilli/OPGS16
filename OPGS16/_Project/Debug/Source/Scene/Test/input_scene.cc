
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/Test/input_scene.h"
#include "../../../Include/Object/InputTest/input_select.h"

namespace debug::scene {

void InputTestScene::Initiate() {
  Instantiate<object::InputSelectObject>("Canvas");
}

} /// ::debug::scene namespace