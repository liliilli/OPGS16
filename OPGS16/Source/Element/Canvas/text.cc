#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Canvas/text.cc
///
/// @brief
/// Definition file of ::opgs16::element::canvas::CText.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-15 Unknown.
/// 2018-04-17 Move definition function body into ::opgs16::element::canvas namespace.
/// 2018-05-28 Remove pointer to implementation idiom.
///

#include <Element/Canvas/text.h>    /// Header file

/// ::opgs16::component::CFont2DRenderer
#include <Component/font2d_renderer.h>
/// ::opgs16::manager::MFontManager
#include <Manager/font_manager.h>

/// import logger in debug mode
#include <Headers/import_logger.h>

namespace opgs16::element::canvas {

using CTextImpl = _internal::CTextImpl;

CText::CText(const std::string& initial_text, const DVector3& position, const DColor& color) :
    m_text{initial_text}, m_color{color} {
  SetWorldPosition(position);

  auto renderer = AddComponent<component::CFont2DRenderer>(*this, u8"", "opFont2d", 0);
  renderer->SetText(m_text);
  renderer->SetColor(color);
  m_component = renderer;

  pUpdateFontScale();
}

void CText::Render() {
  // Set font
  auto font_name = GetFontName();
  if (font_name.empty())
    m_component->SetDefaultFont();
  else
    m_component->SetFont(font_name);

  // Render
  m_component->SetText(GetText());
  m_component->SetColor(GetColor());
  const auto& fp = GetFinalPosition();
  m_component->RenderText(GetOrigin(), glm::vec2{ fp.x, fp.y }, GetAlignment(), m_font_scale);
}

const std::string& CText::GetText() const noexcept {
  return m_text;
}

int32_t CText::GetFontSize() const noexcept {
  return m_font_size;
}

const std::string& CText::GetFontName() const noexcept {
  return m_font_name;
}

const DColor& CText::GetColor() const noexcept {
  return m_color;
}

bool CText::SetFontName(const std::string& font_tag) {
	if (!manager::font::IsFontExist(font_tag)) {
    PUSH_LOG_ERROR_EXT("Font did not find. : [Font : {0}]", font_tag);
    if (!manager::font::GenerateFont(font_tag)) {
      PUSH_LOG_ERROR_EXT("Could not create specified font. [Font : {}]", font_tag);
      m_font_name = "";
      return false;
    }
	}

  m_font_name = font_tag;
  return true;
}

void CText::SetText(const std::string& new_text) noexcept {
  m_text = new_text;
}

void CText::SetFontSize(uint32_t size) noexcept {
  m_font_size = size;
  pUpdateFontScale();
}

void CText::SetColor(const DColor& color) {
  m_color = color;
}

void CText::SetRenderingLayer(int32_t layer_index) {
  m_component->SetRenderingLayer(layer_index);
}

void CText::SetRenderingLayer(const std::string& layer_string) {
  m_component->SetRenderingLayer(layer_string);
}

void CText::pUpdateFontScale() {
  const auto def = manager::font::GetDefaultFontSize();
  m_font_scale = static_cast<float>(m_font_size) / static_cast<float>(def);
}
} /// ::opgs16::element::canvas
