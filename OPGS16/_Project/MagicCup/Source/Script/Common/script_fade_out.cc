
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file`
#include "../../../Include/Script/Common/script_fade_out.h"

#include <Component/procedural2d_renderer.h>
#include <Element/object.h>
#include <Manager/timer_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

namespace magiccup {

void ScriptFadeOut::Initiate() {
  SetComponentActive(false);
}

void ScriptFadeOut::SetTimer(int32_t time_length) {
  m_total = time_length;
}

void ScriptFadeOut::SetCallback(std::function<void()> cb_function) {
  m_callback = cb_function;
}

void ScriptFadeOut::Execute() {
  SetComponentActive(true);
}

void ScriptFadeOut::Start() {
  using opgs16::builtin::s_uniform_alpha;
  using opgs16::component::CProcedural2DRenderer;
  auto& obj = GetBindObject();

  m_wrapper = obj.GetComponent<CProcedural2DRenderer>()->GetWrapper();
  m_wrapper->SetUniformValue<float>(s_uniform_alpha, 0.0f);
}

void ScriptFadeOut::Update(float delta_time) {
  using opgs16::builtin::s_uniform_alpha;

  m_elapsed += static_cast<int32_t>(delta_time * 1'000);

  const float alpha = static_cast<float>(m_elapsed) / m_total;
  m_wrapper->SetUniformValue<float>(s_uniform_alpha, alpha);

  if (m_elapsed >= m_total) {
    SetComponentActive(false);
    OP16_TIMER_SET(m_timer, 500, false, this, &ScriptFadeOut::ExecuteCallback);
  }
}

void ScriptFadeOut::ExecuteCallback() {
  if (m_callback)
    m_callback();
}


} /// ::magiccup namespace