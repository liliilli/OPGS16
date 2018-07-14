#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

#include <vector>

namespace magiccup {
class ScriptStateMachine;
class ObjectCursor;
} /// ::magiccup namespace

namespace magiccup {

class ScriptObjectManagement final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptObjectManagement);

  void EnableCursor();
  void MoveCursorLeft();
  void MoveCursorRight();
  void MoveCursorSelectCup();
  void DisableCursor();

  void FirstStartObjectEffect();
  void StartObjectEffect();

  void ExecuteJudging();
  void ExecuteShaking(int32_t shaking_count);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  void ExecuteTransitionShaking();
  void ExecuteTransitionSelect();
  void NextShake();

  void Judge();

  std::vector<opgs16::element::CObject*> m_item_list;

  opgs16::element::CTimerHandle m_effect_timer;
  opgs16::element::CTimerHandle m_shaking_timer;

  ObjectCursor* m_cursor = nullptr;
  ScriptStateMachine* m_state_machine = nullptr;

  int32_t m_ball_index = 1;
  int32_t m_shaking_count_on_stage = -1;
  int32_t m_cursor_index = 1;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_OBJ_MNG_H