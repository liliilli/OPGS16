
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/Test/rendering.h"

#include "../../../Include/Object/RenderingTest/manager_canvas.h"

namespace debug::scene {

void RenderingTestScene::Initiate() {
  Instantiate<object::RenderingTestManagerCanvas>("Canvas");
}

}