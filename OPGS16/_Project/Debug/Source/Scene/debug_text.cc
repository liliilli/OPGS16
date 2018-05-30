
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Scene/debug_text.cc
///

/// Header file
#include "../../Include/Scene/debug_text.h"

#include "../../Include/Object/empty_canvas.h"
#include "../../Include/Script/text_script.h"

namespace debug::scene {

void DebugText::Initiate() {
  auto canvas = Instantiate<object::EmptyCanvas>("Manager");
  canvas->AddComponent<script::TextScript>(*canvas);
}

} /// ::debug::scene