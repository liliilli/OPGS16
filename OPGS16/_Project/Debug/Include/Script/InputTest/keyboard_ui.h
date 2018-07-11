#ifndef DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_UI_H
#define DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_UI_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace debug::script {

class KeyboardUiScript final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(KeyboardUiScript);

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override final;

  opgs16::element::CObject* m_obj_keyboard_ui = nullptr;
  opgs16::element::CObject* m_obj_keyboard_log = nullptr;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_UI_H