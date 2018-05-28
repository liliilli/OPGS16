
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/font2d_renderer.cc
///
/// @brief
///
///
/// @author Jongmin Yun
/// @log
/// 2018-05-28 Create file.
///

/// Header file
#include <Component/font2d_renderer.h>

#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <Core/core_setting.h>

/// ::opgs16::manager::MShaderManager
#include <Manager/shader_manager.h>
/// ::opgs16::manager::_internal
#include <Manager/Internal/font_internal.h>

namespace {

std::vector<std::string> SeparateTextToList(const std::string& text);

void Render(const opgs16::manager::_internal::Character& ch_info,
            GLuint m_vbo,
            const std::array<float, 24>& vertices);

///
/// @brief
/// The method separate input to multi-lines strings
/// detecting line-feed return carriage.
///
/// @param[in] text String text to separate
///
/// @return string list.
///
std::vector<std::string> SeparateTextToList(const std::string& text) {
  std::vector<std::string> result;

  std::stringstream stream{ text };
  for (std::string line; std::getline(stream, line);)
    result.emplace_back(line);

  return result;
}

/**
 * @brief The method gets text and returns total rendering width size.
 *
 * @param[in] text One line string to measure.
 * @param[in] font_set
 *
 * @return The size
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
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

/**
 * @brief The method gets character quad vertices to be needed for rendering.
 *
 * @param[in] ch_info Specific character glyph information.
 * @param[in] position The position that character which will be rendered.
 * @param[in] scale Scale value to magnify or minify character render size.
 *
 * @return Character glyph render vertices information.
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
std::array<float, 24> GetCharacterVertices(
    const opgs16::manager::_internal::Character& ch_info,
    const glm::vec2& position,
    const float scale) {
  const auto x_offset = ch_info.bearing.x * scale;
  const auto y_offset = (ch_info.size.y - ch_info.bearing.y) * scale;
  const glm::vec2 ch_pos = position + glm::vec2{ x_offset, -y_offset };

  auto w = ch_info.size.x;
  auto h = ch_info.size.y;
  if (scale != 1.0f) {
    if (scale == 0.5f) {
      w >>= 1;
      h >>= 1;
    }
    else if (scale == 2.0f) {
      w <<= 1;
      h <<= 1;
    }
    else {
      w *= static_cast<int>(scale);
      h *= static_cast<int>(scale);
    }
  }

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
void RenderLeftSide(const std::vector<std::string>& container,
                    opgs16::manager::font::font_type* font_set,
                    const glm::vec2& position,
                    const GLuint m_vbo,
                    const float scale) {
  auto pos = position;

  const uint32_t text_container_length = static_cast<uint32_t>(container.size());
  for (uint32_t i = 0; i < text_container_length; ++i) {
    for (const auto& chr : container[i]) {
      const auto& ch_info = (*font_set)[chr];
      Render(ch_info, m_vbo, GetCharacterVertices(ch_info, pos, scale));
      pos.x += (ch_info.advance >> 6) * scale;
    }

    pos.x = position.x;
    pos.y -= (*font_set)[0].size.y * 1.5f;
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
    pos.y -= (*font_set)[0].size.y * 1.5f;
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
    pos.y -= (*font_set)[0].size.y * 1.5f;
  }
}

/**
 * @brief Actual render method. This method must be called in Render__Side() method.
 *
 * @param[in] ch_info
 * @param[in] m_vbo
 * @param[in] vertices
 */
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

/**
 * @brief The method sets VAO, VBO to render string on screen.
 *
 * This methods called when initiate instance.
 */
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

} /// unname namespace

namespace opgs16::component {

CFont2DRenderer::CFont2DRenderer(element::CObject& bind_object,
                                 const std::string& font_tag,
                                 const std::string& shader_tag,
                                 const uint32_t rendering_layer) :
    CRendererBase(bind_object, rendering_layer) {
  /// Function Body
  m_projection = {
      glm::ortho(0.f, static_cast<float>(SGlobalSetting::ScreenWidth()),
      0.f,
      static_cast<float>(SGlobalSetting::ScreenHeight())) };
  BindVertexAttributes(&m_vao, &m_vbo);

  m_wrapper.SetShader(manager::ShaderManager::Instance().Shader(shader_tag));

  SetFont(font_tag);
}

void CFont2DRenderer::RenderTextNew(const std::string& text,
                                    IOriginable::Origin origin,
                                    const glm::vec2 final_position,
                                    const glm::vec3 color,
                                    IAlignable::Alignment alignment,
                                    const float scale) {
  if (m_dirty == _internal::EStringCacheFlag::Dirty) {
    RefreshStringContainers(text);
    m_dirty = _internal::EStringCacheFlag::Clean;
  }

  m_wrapper.SetUniformValue("textColor", color);
  m_wrapper.SetUniformValue("projection", m_projection);
  glBindVertexArray(m_vao);
  m_wrapper.UseShader();

  switch (typedef IAlignable::Alignment Align; alignment) {
  case Align::LEFT:
    RenderLeftSide(m_text_container, m_font_set, final_position, m_vbo, scale);
    break;
  case Align::CENTER:
    RenderCenterSide(m_text_container, m_text_render_width,
                     m_font_set, final_position, m_vbo, scale);
    break;
  case Align::RIGHT:
    RenderRightSide(m_text_container, m_text_render_width,
                    m_font_set, final_position, m_vbo, scale);
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
  }
  else {
    m_font_set = manager::font::GetDefaultFont();
  }
}

void CFont2DRenderer::RefreshStringContainers(const std::string& text) {
  m_text_container.clear();
  m_text_render_width.clear();

  if (std::string::npos == text.find('\n')) {
    m_text_container.emplace_back(text);
    m_text_render_width.push_back(
        CalculateStringRenderWidth(m_text_container[0], m_font_set));
  }
  else {
    m_text_container = SeparateTextToList(text);
    for (const auto& text_line : m_text_container)
      m_text_render_width.push_back(
          CalculateStringRenderWidth(text_line, m_font_set));
  }
}


} /// ::opgs16::component namespace