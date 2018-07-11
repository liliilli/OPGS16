
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/InputTest/keyboard_ui.h"

#include <Element/Canvas/canvas.h>

#include "../../../Include/Object/InputTest/keyboard.h"
#include "../../../Include/Object/InputTest/keyboard_log.h"

namespace debug::script {

void KeyboardUiScript::Initiate() {
  using opgs16::element::canvas::CCanvas;

  auto& canvas_obj = GetBindObject();
  auto keyboard_ui = canvas_obj.Instantiate<object::KeyboardUiObject>(
      "KeyboardUi", static_cast<CCanvas*>(&canvas_obj)
  );
  auto keyboard_log = canvas_obj.Instantiate<object::KeyboardLogObject>(
      "KeyboardLog", static_cast<CCanvas*>(&canvas_obj)
  );

  m_obj_keyboard_ui = keyboard_ui;
  m_obj_keyboard_log = keyboard_log;
}

void KeyboardUiScript::Update(float delta_time) {

}

void KeyboardUiScript::Destroy() {
  auto& bind_obj = GetBindObject();

  if (m_obj_keyboard_ui) {
    bind_obj.DestroyChild(*m_obj_keyboard_ui);
  }

  if (m_obj_keyboard_log) {
    bind_obj.DestroyChild(*m_obj_keyboard_log);
  }
}

} /// ::debug::script namespace