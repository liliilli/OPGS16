#ifndef MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_TRANSITION_MANAGE_H
#define MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_TRANSITION_MANAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <array>

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {

class ScriptTransitionManagement final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptTransitionManagement);

  void SwitchOnSequence();

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  std::array<opgs16::element::CObject*, 3> m_sequences;

  opgs16::element::CTimerHandle m_timer;

  bool m_on_focused  = false;
  int32_t m_sequence = 0;
  int32_t m_seq_size = 2;

  int32_t m_elapsed = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEMAIN_SCRIPT_TRANSITION_MANAGE_H