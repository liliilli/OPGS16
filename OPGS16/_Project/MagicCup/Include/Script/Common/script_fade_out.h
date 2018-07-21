#ifndef MAGICCUP_SCRIPT_COMMON_FADE_OUT_H
#define MAGICCUP_SCRIPT_COMMON_FADE_OUT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <functional>

#include <Component/script_frame.h>
#include <Frame/timer_handle.h>

namespace magiccup {

class ScriptFadeOut final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ScriptFadeOut);

  void SetTimer(int32_t time_length);

  void SetCallback(std::function<void(void)> cb_function);

  void SetIntensity(float intensity);

  void Execute();

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;

  void ExecuteCallback();

  opgs16::element::CShaderWrapper* m_wrapper = nullptr;

  std::function<void(void)> m_callback = nullptr;

  int32_t m_elapsed = 0;
  int32_t m_total   = 1;
  float m_intensity = 0.0f;

  opgs16::element::CTimerHandle m_timer;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCRIPT_COMMON_FADE_OUT_H