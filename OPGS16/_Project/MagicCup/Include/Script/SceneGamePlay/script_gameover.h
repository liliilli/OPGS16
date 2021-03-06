#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_GAMEOVER_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_GAMEOVER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {

class ScriptGameOverSequence final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptGameOverSequence);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  void CbReturnToMainScene();

  opgs16::element::CTimerHandle m_return_timer;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_GAMEOVER_H