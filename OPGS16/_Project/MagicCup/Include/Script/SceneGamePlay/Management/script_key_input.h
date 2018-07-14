#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_KEY_INPUT_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_KEY_INPUT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {
class ScriptStateMachine;
class ScriptObjectManagement;
} /// Forward declaration

namespace magiccup {

class ScriptKeyInput final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptKeyInput);

  void EnableSelectKeyInput();
  void DisableSelectKeyInput();

private:
  void Start() override;
  void Update(float delta_time) override final;

  bool is_start_initiated = false;
  bool is_select_key_enabled = false;

  ScriptStateMachine* m_state_machine = nullptr;
  ScriptObjectManagement* m_obj_management = nullptr;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_KEY_INPUT_H