#ifndef MAGICCUP_SCRIPT_SCENEGAMELOGO_SCRIPT_CHAR_JUMPING_H
#define MAGICCUP_SCRIPT_SCENEGAMELOGO_SCRIPT_CHAR_JUMPING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace opgs16::element::canvas {
class CText;
} /// ::opgs16::element::canvas namespace

namespace magiccup {

class ScriptCharJumping final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptCharJumping);

  void SetChar(char character);

  void ExecuteEffect(int32_t delay);

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  void EnableEffect();

  opgs16::element::canvas::CText* m_text = nullptr;

  opgs16::element::CTimerHandle m_delay_timer;
  int32_t m_elapsed = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMELOGO_SCRIPT_CHAR_JUMPING_H
