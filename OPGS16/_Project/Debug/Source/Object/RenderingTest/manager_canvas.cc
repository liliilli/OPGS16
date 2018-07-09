
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/RenderingTest/manager_canvas.h"

#include "../../../Include/Script/RenderingTest/select_menu.h"

namespace debug::object {

RenderingTestManagerCanvas::RenderingTestManagerCanvas() {
  AddComponent<script::RenderingTestSelectMenu>(*this);
}

} /// ::debug::object namespace