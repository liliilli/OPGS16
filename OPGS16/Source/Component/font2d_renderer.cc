
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/font2d_renderer.cc
///
/// @brief
/// Implementation file of ::opgs16::component::CFont2DRenderer.
///
/// @author Jongmin Yun
/// @log
/// 2018-05-28 Create file.
///

/// Header file
#include <Component/font2d_renderer.h>

#include <sstream>

/// @todo Deprecated since C++17. Replace logic with other alternatives.
#include <codecvt>
#include <locale>

#include <glm/gtc/matrix_transform.hpp>
#include <Phitos/Utf/ustring.h>

/// ::opgs16::core::
#include <Core/core_setting.h>
/// ::opgs16::manager::MShaderManager
#include <Manager/shader_manager.h>
/// ::opgs16::manager::_internal
#include <Manager/Internal/font_internal.h>

using Utf16TextContainer =
    opgs16::component::CFont2DRenderer::Utf16TextContainer;

namespace {

std::vector<std::u16string> SeparateUtf8TextToUtf16StringList(
    const std::string& text);

void Render(const opgs16::manager::_internal::Character& ch_info,
            GLuint m_vbo,
            const std::array<float, 24>& vertices);

///
/// @brief
/// The method sets VAO, VBO to render string on screen.
/// This methods called when initiate instance.
///
/// @todo Make Geometry manager, set Font2DQuad geometry array, buffer which can
/// be used by default.
///
void BindVertexAttributes(GLuint* m_vao, GLuint* m_vbo) {
  glGenVertexArrays(1, m_vao);
  glGenBuffers(1, m_vbo);
  glBindVertexArray(*m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, *m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, nullptr, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

///
/// @brief
///
std::u16string ConvertUtf8ToUtf16(const std::string& string) {
  const auto utf8_char_array = string.c_str();

  std::u16string result_utf16_line_string;
  result_utf16_line_string.reserve(
      phitos::utf::GetUtf16LengthFrom(utf8_char_array));

  uint32_t i = 0;
  for (auto it = utf8_char_array;
       *it != '\0';
       it += phitos::utf::GetByteOfUtf8Char(it)) {
    result_utf16_line_string.push_back(phitos::utf::GetRawUtf16CharacterFrom(it));
  }

  return std::move(result_utf16_line_string);

#ifdef false
  return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.
      from_bytes(string);
#endif
}

///
/// @brief
/// The method separate input to multi-lines strings
/// detecting line-feed return carriage.
///
/// @param[in] text String text to separate
///
/// @return string list.
///
std::vector<std::u16string> SeparateUtf8TextToUtf16StringList(
    const std::string& text) {
  std::vector<std::u16string> result;

  std::stringstream stream{ text };
  for (std::string line; std::getline(stream, line);)
    result.emplace_back(ConvertUtf8ToUtf16(line));

  return std::move(result);
}

///
/// @brief The method gets text and returns total rendering width size.
///
/// @param[in] text One line string to measure.
/// @param[in] font_set
///
/// @return The size
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
uint32_t CalculateStringRenderWidth(
    const std::string& text,
    opgs16::manager::font::font_type* font_set) {
  uint32_t result_width = 0;
  for (const char chr : text) {
    const auto& ch_info = (*font_set)[chr];
    result_width += static_cast<uint32_t>((ch_info.advance >> 6));
  }

  return result_width;
}

///
/// @brief The method gets character quad vertices to be needed for rendering.
///
/// @param[in] ch_info Specific character glyph information.
/// @param[in] position The position that character which will be rendered.
/// @param[in] scale Scale value to magnify or minify character render size.
///
/// @return Character glyph render vertices information.
/// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
///
std::array<float, 24> GetCharacterVertices(
    const opgs16::manager::_internal::Character& ch_info,
    const glm::vec2& position,
    const float scale) {
  const auto x_offset = ch_info.bearing.x * scale;
  const auto y_offset = (ch_info.size.y - ch_info.bearing.y) * scale;
  const glm::vec2 ch_pos = position + glm::vec2{ x_offset, -y_offset };

  const auto w = ch_info.size.x * scale;
  const auto h = ch_info.size.y * scale;

  return std::array<float, 24>{
      ch_pos.x, ch_pos.y + h, 0.f, 0.f,
      ch_pos.x, ch_pos.y, 0.f, 1.f,
      ch_pos.x + w, ch_pos.y, 1.f, 1.f,
      ch_pos.x, ch_pos.y + h, 0.f, 0.f,
      ch_pos.x + w, ch_pos.y, 1.f, 1.f,
      ch_pos.x + w, ch_pos.y + h, 1.f, 0.f
  };
}

///
/// @brief Final render method actually renders strings from left side.
///
/// @param[in] container Container stores multi-lined (separated) strings.
/// @param[in] position Position on which to render.
/// @param[in] scale Scale factor, it magnify or minify rendered string textures.
///
void RenderLeftSide(const Utf16TextContainer& container,
                    const std::string& font_name,
                    opgs16::manager::font::font_type* font_set,
                    const glm::vec2& position,
                    const GLuint m_vbo,
                    const float scale) {
  auto pos = position;

  for (const auto& utf16_string : container) {
    for (const char16_t chr : utf16_string) {
      // If character on font is not exist yet, generate new one.
      if (font_set->find(chr) == font_set->end())
        opgs16::manager::font::GenerateCharacter(font_name, chr);

      const auto& ch_info = (*font_set)[chr];
      Render(ch_info, m_vbo, GetCharacterVertices(ch_info, pos, scale));
      pos.x += (ch_info.advance >> 6) * scale;
    }

    pos.x = position.x;
    pos.y -= static_cast<int>((*font_set)['0'].size.y * scale * 1.5f);
  }
}

void RenderCenterSide(const std::vector<std::string>& container,
                      const std::vector<uint32_t>& text_render_width,
                      opgs16::manager::font::font_type* font_set,
                      const glm::vec2& position,
                      const GLuint m_vbo,
                      const float scale) {
  auto pos = position;

  const uint32_t text_container_length = static_cast<uint32_t>(container.size());
  for (uint32_t i = 0; i < text_container_length; ++i) {
    pos.x -= static_cast<uint32_t>(text_render_width[i] * scale) >> 1;

    for (const auto& chr : container[i]) {
      const auto& ch_info = (*font_set)[chr];
      Render(ch_info, m_vbo, GetCharacterVertices(ch_info, pos, scale));
      pos.x += (ch_info.advance >> 6) * scale;
    }

    pos.x = position.x;
    pos.y -= (*font_set)['0'].size.y * 1.5f;
  }
}

void RenderRightSide(const std::vector<std::string>& container,
                     const std::vector<uint32_t>& text_render_width,
                     opgs16::manager::font::font_type* font_set,
                     const glm::vec2& position,
                     const GLuint m_vbo,
                     const float scale) {
  auto pos = position;

  const uint32_t text_container_length = static_cast<uint32_t>(container.size());
  for (uint32_t i = 0; i < text_container_length; ++i) {
    pos.x -= static_cast<uint32_t>(text_render_width[i] * scale);

    for (const auto& chr : container[i]) {
      const auto& ch_info = (*font_set)[chr];
      Render(ch_info, m_vbo, GetCharacterVertices(ch_info, pos, scale));
      pos.x += (ch_info.advance >> 6) * scale;
    }

    pos.x = position.x;
    pos.y -= (*font_set)['0'].size.y * 1.5f;
  }
}

///
/// @brief Actual render method. This method must be called in Render__Side() method.
///
/// @param[in] ch_info
/// @param[in] m_vbo
/// @param[in] vertices
///
void Render(const opgs16::manager::_internal::Character& ch_info,
            GLuint m_vbo,
            const std::array<float, 24>& vertices) {
	// Render texture glyph
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ch_info.texture_id);

	// Update content of VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

} /// unname namespace

namespace opgs16::component {

CFont2DRenderer::CFont2DRenderer(element::CObject& bind_object,
                                 const std::string& font_tag,
                                 const std::string& shader_tag,
                                 const uint32_t rendering_layer) :
    CRendererBase(bind_object, rendering_layer), m_font_name{font_tag} {
  /// Function Body
  SetProjectionMatrix({
      glm::ortho(0.f, static_cast<float>(SGlobalSetting::ScreenWidth()),
      0.f,
      static_cast<float>(SGlobalSetting::ScreenHeight())) });
  BindVertexAttributes(&m_vao, &m_vbo);

  m_wrapper.SetShader(manager::shader::GetShader(shader_tag));

  SetFont(font_tag);
}

void CFont2DRenderer::RenderText(IOriginable::Origin origin,
                                 const glm::vec2 final_position,
                                 IAlignable::Alignment alignment,
                                 const float scale) {
  /// Ready
  if (m_string_dirty == _internal::EDirtyFlag::Dirty) {
    RefreshStringContainers(m_temporary_utf8_string);
    m_temporary_utf8_string.clear();
    m_string_dirty = _internal::EDirtyFlag::Clean;
  }
  if (m_unicode_text_container.empty())
    return;

  if (m_color_dirty == _internal::EDirtyFlag::Dirty) {
    m_wrapper.SetUniformValue("textColor", m_color);
    m_color_dirty = _internal::EDirtyFlag::Clean;
  }
  if (m_proj_matrix_dirty == _internal::EDirtyFlag::Dirty) {
    m_wrapper.SetUniformValue("projection", m_projection);
    m_proj_matrix_dirty = _internal::EDirtyFlag::Clean;
  }

  /// Body
  glBindVertexArray(m_vao);
  m_wrapper.UseShader();

  switch (typedef IAlignable::Alignment Align; alignment) {
  case Align::LEFT:
    RenderLeftSide(m_unicode_text_container, m_font_name, m_font_set, final_position, m_vbo, scale);
    break;
#ifdef false
  case Align::CENTER:
    RenderCenterSide(m_text_container, m_text_render_width,
                     m_font_set, final_position, m_vbo, scale);
    break;
  case Align::RIGHT:
    RenderRightSide(m_text_container, m_text_render_width,
                    m_font_set, final_position, m_vbo, scale);
    break;
#endif
  default:
    NEU_NOT_IMPLEMENTED_ASSERT();
    break;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void CFont2DRenderer::SetDefaultFont() {
  m_font_set = manager::font::GetDefaultFont();
}

void CFont2DRenderer::SetFont(const std::string& font_name) {
  if (auto font_set = manager::font::GetFontSetPtr(font_name);
      font_set.has_value()) {
    m_font_set = *font_set;
    m_font_name = font_name;
  }
  else {
    m_font_set = manager::font::GetDefaultFont();
  }
}

void CFont2DRenderer::SetText(const std::string& utf8_text) {
  m_temporary_utf8_string = utf8_text;
  m_string_dirty = _internal::EDirtyFlag::Dirty;
}

void CFont2DRenderer::SetColor(const glm::vec3& color) {
  for (uint32_t i = 0; i < 3; ++i) {
    if (color[i] < 0) {
      m_color[i] = 0.0f;
      continue;
    }
    if (color[i] > 1) {
      m_color[i] = 1.0f;
      continue;
    }

    m_color[i] = color[i];
  }
  m_color_dirty = _internal::EDirtyFlag::Dirty;
}

void CFont2DRenderer::SetProjectionMatrix(const glm::mat4& projection_matrix) {
  m_proj_matrix_dirty = _internal::EDirtyFlag::Dirty;
  m_projection = projection_matrix;
}

void CFont2DRenderer::RefreshStringContainers(const std::string& text) {
  m_unicode_text_container.clear();
  m_text_render_width.clear();

  if (std::string::npos == text.find('\n')) {
    m_unicode_text_container.emplace_back(ConvertUtf8ToUtf16(text));
    m_text_render_width.push_back(
        static_cast<uint32_t>(m_unicode_text_container[0].length()));
  }
  else {
    m_unicode_text_container = SeparateUtf8TextToUtf16StringList(text);
    for (const auto& text_line : m_unicode_text_container)
      m_text_render_width.push_back(
          static_cast<uint32_t>(text_line.length()));
  }
}


} /// ::opgs16::component namespace