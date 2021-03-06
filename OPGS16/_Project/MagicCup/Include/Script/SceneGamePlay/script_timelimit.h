#ifndef MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_TIMELIMIT_H
#define MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_TIMELIMIT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace magiccup {

class ScriptUiTimelimit final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptUiTimelimit);

  void ExecuteTimeLimit(int32_t time_value);

  void HaltTimeLimit();

  int32_t GetTimeValue() const noexcept;

private:
  void Initiate() override final;
  void Update(float delta_time) override final;

  opgs16::element::canvas::CImage*  m_timer_bar = nullptr;
  opgs16::element::canvas::CText*   m_timer_text = nullptr;

  const int32_t m_initial_width = 128;
  int32_t m_time_value = 0;
  int32_t m_time_set = 0;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_SCENEGAMEPLAY_SCRIPT_TIMELIMIT_H