#ifndef MAGICCUP_SCRIPT_SCOREBOARD_RANK_TEXT_DIVINGIN_H
#define MAGICCUP_SCRIPT_SCOREBOARD_RANK_TEXT_DIVINGIN_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>
#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16 {
  struct DVector3;
}

//!
//! Implementation
//!

namespace magiccup {

class ScriptSequenceMoving final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptSequenceMoving);

  void ExecuteMovingEffect(int32_t time, bool is_vanished);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  int32_t m_time = 0;
  int32_t m_elapsed = 0;
  int32_t m_vanished = false;

  opgs16::DVector3 m_initial_position = {};
  opgs16::DVector3 m_destination = opgs16::DVector3{};

  opgs16::element::CTimerHandle m_timer;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCOREBOARD_RANK_TEXT_DIVINGIN_H