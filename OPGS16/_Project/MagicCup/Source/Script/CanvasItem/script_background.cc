
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/CanvasItem/script_background.h"

#include <Component/sprite2d_renderer.h>
#include <Element/object.h>
#include <Manager/Internal/shader_builtin_keywords.h>

namespace magiccup {

void ScriptUiBackground::Initiate() {
  using opgs16::component::CSprite2DRenderer;
  auto renderer = GetBindObject().GetComponent<CSprite2DRenderer>();
  m_wrapper = &renderer->GetWrapper();
}

void ScriptUiBackground::Update(float delta_time) {
  using namespace opgs16::builtin;

  m_elapsed += delta_time * 0.5f;
  m_wrapper->SetUniformValue<glm::vec2>(
      s_uniform_offset,
      {m_elapsed * m_mov_x, m_elapsed * m_mov_y});
}

void ScriptUiBackground::SetTile(int32_t tile_index) {
  using opgs16::component::CSprite2DRenderer;
  GetBindObject().GetComponent<CSprite2DRenderer>()
      ->SetTextureFragmentIndex(tile_index);
}

void ScriptUiBackground::SetOffset(const float x, const float y) noexcept {
  m_mov_x = x;
  m_mov_y = y;
}

void ScriptUiBackground::SetScale(const float x, const float y) noexcept {
  using namespace opgs16::builtin;

  m_wrapper->SetUniformValue<glm::vec2>(
    s_uniform_scale, {x, y});
}

} /// ::magiccup namespace