#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_STATE_MACHINE_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_STATE_MACHINE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

#include "../../../../Include/Internal/state_machine.h"

namespace magiccup {
class ScriptDataContainer;
class ScriptObjectManagement;
class ScriptKeyInput;
} /// Forward declarations

namespace magiccup {

class ScriptStateMachine final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptStateMachine);

  void TransitShaking();
  void TransitSelect();
  void TransitResult();

  void TransitGameState(EGameState new_state);

private:
  void Initiate() override final;
  void Start() override final;

  void ReturnToTitle();

  magiccup::EGameState m_state = EGameState::None;

  ScriptDataContainer* m_data = nullptr;
  ScriptObjectManagement* m_object_management = nullptr;
  ScriptKeyInput* m_input = nullptr;

  opgs16::element::CTimerHandle m_gameover_effect_timer;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_MANAGEMENT_SCRIPT_STATE_MACHINE_H