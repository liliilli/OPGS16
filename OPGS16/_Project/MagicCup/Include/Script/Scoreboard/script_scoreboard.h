#ifndef MAGICCUP_SCRIPT_SCOREBOARD_SCRIPT_SCOREBOARD_H
#define MAGICCUP_SCRIPT_SCOREBOARD_SCRIPT_SCOREBOARD_H

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

namespace opgs16::element::canvas {
class CText;
}

namespace magiccup {
class ScriptTile9Frame;
}

//!
//! Implementation
//!

namespace magiccup {

class ScriptScoreboard final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptScoreboard);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;
  void Destroy() override final;

  void ExecuteVanishScoreboard();

  ScriptTile9Frame* m_frame = nullptr;
  std::array<opgs16::element::canvas::CText*, 10> m_text;

  opgs16::element::CTimerHandle m_cutdown_timer;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCOREBOARD_SCRIPT_SCOREBOARD_H