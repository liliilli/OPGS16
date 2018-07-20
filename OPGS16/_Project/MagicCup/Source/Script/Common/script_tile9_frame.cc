
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/Common/script_tile9_frame.h"

#include <Component/sprite2d_renderer.h>
#include <Element/Canvas/image.h>
#include <Manager/timer_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

namespace magiccup {

void ScriptTile9Frame::Initiate() {
  using opgs16::element::canvas::CImage;
  using opgs16::component::CSprite2DRenderer;

  m_object    = static_cast<CImage*>(&GetBindObject());
  m_renderer  = m_object->GetComponent<CSprite2DRenderer>();
  m_renderer_wrapper = &m_renderer->GetWrapper();

  this->SetComponentActive(false);
}

void ScriptTile9Frame::SetImageSize(opgs16::DVectorInt2 size) {
  m_frame_size      = size;
  m_uniform_dirty   = true;
}

void ScriptTile9Frame::SetEdgeImageSize(int32_t pixel_value) {
  m_edge_size       = opgs16::DVectorInt2{pixel_value, pixel_value};
  m_renderer_wrapper->SetUniformInt("uEdgeSize", pixel_value);
  m_uniform_dirty   = true;
}

void ScriptTile9Frame::SetInterval(int32_t length) {
  m_effect_interval = length;
  m_uniform_dirty   = true;
}

void ScriptTile9Frame::SetAlpha(float alpha) {
  m_renderer_wrapper->SetUniformFloat(opgs16::builtin::s_uniform_alpha, alpha);
}

void ScriptTile9Frame::Execute(bool is_openup) {
  if (m_is_executing) {
    PUSH_LOG_WARN("ScriptTile9Frame Execute() is not called simutaneously.");
    return;
  }

  m_is_openup = is_openup;
  m_is_executing = true;

  if (m_is_openup) {
    m_object->SetImageSize(0, 0);
  }
  else {
    m_object->SetImageSize(m_frame_size.x, m_frame_size.y);
  }

  this->SetComponentActive(true);
}

void ScriptTile9Frame::Update(float delta_time) {
  m_elapsed += delta_time * 1'000;
  const float ratio = static_cast<float>(m_elapsed) / m_effect_interval;

  if (m_elapsed >= m_effect_interval) {
    if (m_is_openup)
      m_object->SetImageSize(m_frame_size.x, m_frame_size.y);
    else
      m_object->SetImageSize(0, 0);

    m_is_executing = false;
    m_elapsed = 0;
    this->SetComponentActive(false);
    return;
  }

  if (m_is_openup) {
    auto x = m_frame_size.x * ratio;
    auto y = m_frame_size.y * ratio;

    m_object->SetImageSize(x, y);
    m_renderer_wrapper->SetUniformVec2("uPresentSize", {x, y});
  }
  else {
    auto x = m_frame_size.x * (1 - ratio);
    auto y = m_frame_size.y * (1 - ratio);

    m_object->SetImageSize(x, y);
    m_renderer_wrapper->SetUniformVec2("uPresentSize", {x, y});
  }
}

bool ScriptTile9Frame::IsExecutionDone() const noexcept {
  return !this->IsComponentActive();
}

} /// ::magiccup namespace