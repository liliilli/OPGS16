#ifndef MAGICCUP_SCRITP_SCENEGAMEPLAY_GAMEOVER_SCRIPT_NAMEENTRY_H
#define MAGICCUP_SCRITP_SCENEGAMEPLAY_GAMEOVER_SCRIPT_NAMEENTRY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <array>

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

//!
//! Forward declaration
//!

namespace magiccup {
class ScriptCharRoulette;
}

//!
//! Implementation
//!

namespace magiccup {

class ScriptNameEntry final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptNameEntry);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  void CbReturnToMainScene();

  std::array<ScriptCharRoulette*, 3> m_characters;
  opgs16::element::CTimerHandle m_return_timer;
  int32_t m_focus_index = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRITP_SCENEGAMEPLAY_GAMEOVER_SCRIPT_NAMEENTRY_H