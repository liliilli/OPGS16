
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../Include/Scene/select_scene.h"

#include "../../Include/Object/SceneSelect/select_manager.h"
#include "Element/Interface/i_alignable.h"
#include <Element/Canvas/text.h>

namespace debug::scene {

void SelectScene::Initiate() {
  auto canvas = Instantiate<object::SelectManager>("Canvas");
}

} /// ::debug::scene namespace