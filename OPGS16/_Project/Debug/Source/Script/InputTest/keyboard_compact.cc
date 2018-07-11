
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/InputTest/keyboard_compact.h"

#include <Element/object.h>

#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Script/InputTest/keyboard_ui.h"

namespace debug::script {

void KeyboardCompactTest::Initiate() {
  auto& canvas_obj = GetBindObject();

  auto subject = canvas_obj.Instantiate<object::Subject>("Subject");
  subject->SetText("Keyboard input test");

  canvas_obj.AddComponent<script::KeyboardUiScript>(canvas_obj);
}

} /// ::debug::script namespace