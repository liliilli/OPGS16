
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/ui_tile9_dialogue_frame.h"

#include <Component/sprite2d_renderer.h>

#include "../../../Include/Script/Common/script_tile9_frame.h"

namespace magiccup {

UiTile9DialogueFrame::UiTile9DialogueFrame(
    const std::string& sprite_tag,
    const opgs16::element::canvas::CCanvas* ref_canvas) :
    CImage{sprite_tag, ref_canvas} {
  m_script = this->AddComponent<ScriptTile9Frame>(*this);

  using opgs16::component::CSprite2DRenderer;
  this->GetComponent<CSprite2DRenderer>()->SetShader(s_shader_name);
}

void UiTile9DialogueFrame::SetFullImageSize
    (const opgs16::DVectorInt2& size_vector) noexcept {
  m_script->SetImageSize(size_vector);
}

void UiTile9DialogueFrame::SetEdgeImageSize(int32_t pixel_value) noexcept {
  m_script->SetEdgeImageSize(pixel_value);
}

void UiTile9DialogueFrame::SetInterval(int32_t length) noexcept {
  m_script->SetInterval(length);
}

void UiTile9DialogueFrame::SetAlpha(float alpha) noexcept {
  m_script->SetAlpha(alpha);
}

void UiTile9DialogueFrame::Execute(bool is_openup) noexcept {
  m_script->Execute(is_openup);
}

ScriptTile9Frame* UiTile9DialogueFrame::GetScript() const {
  return m_script;
}

} /// ::magiccup namespace