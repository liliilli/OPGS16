#ifndef OPGS16_COMPONENT_FONT2D_RENDERER_H
#define OPGS16_COMPONENT_FONT2D_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/font2d_renderer.h
///
/// @brief
/// Component which displays text string on display with designated font glyphs,
/// projection matrix, 2d position, and color etc.
///
/// @author Jongmin Yun
/// @log
/// 2018-05-28 Create file.
///

/// ::opgs16::component::_internal::CRendererBase
#include <Component/Internal/renderer_base.h>
/// ::opgs16::component::_internal enum flags.
#include <Component/Internal/flag.h>
/// ::opgs16::element::IOriginable::Origin
#include <Element/Interface/i_originable.h>
/// ::opgs16::element::IAlignable::Alignment
#include <Element/Interface/i_alignable.h>
/// ::opgs16::manager::font namespace.
#include <Manager/font_manager.h>

/// Component macroes
#include <Component/Internal/component_macro.h>

namespace opgs16::component {

class CFont2DRenderer final : public _internal::CRendererBase {
public:
  using Utf16TextContainer = std::vector<std::u16string>;

public:
  CFont2DRenderer(element::CObject& bind_object,
                  const std::string& font_tag,
                  const std::string& shader_tag,
                  const uint32_t rendering_layer = 0);

  ///
  /// @brief The method renders given text on given position with given color.
  ///
  /// This get text rendered with relative position from origin with color by aligning.
  /// If text is multilined, text will be tokenized with '\n' return-carriage character.
  ///
  /// @param[in] origin Origin position from which text strings rendered.
  /// position bound is [0, screen_size], so DOWN_LEFT has position (0, 0) in Screen space.
  /// In contrast UP_RIGHT will be (width, height) in Screen space.
  ///
  /// @param[in] final_position Relatve position from origin position string will be rendered.
  /// Final position string rendered is (x, y) = (origin + relative_position + alignment_offset)
  ///
  /// @param[in] alignment String alignment parameter. default value is LEFT. (left-side)
  /// @param[in] scale Scale factor value to apply it. Default value is 1.0f. (not-change)
  ///
  /// @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
  ///
  void RenderText(IOriginable::Origin origin,
                  const glm::vec2 final_position,
                  IAlignable::Alignment alignment,
                  const float scale);

  ///
  /// @brief
  /// Set renderer to display text by using default font.
  /// If default font is not found, halt and output error.
  /// OPGS16 must manifested default font; otherwise game will not be executed.
  ///
  void SetDefaultFont();

  ///
  /// @brief
  /// Get CFont2DRenderer displayed by using specific font called by font_name.
  /// If specified font name is not found in font information container,
  /// It will replaced with default font by calling SetDefaultFont().
  ///
  /// @param[in] font_name Font name to use on this rendering component.
  ///
  void SetFont(const std::string& font_name);

  ///
  /// @brief
  /// Update rendering text which is going to be displayed on screen
  /// next render frame if only component and bound object are activated.
  ///
  /// @param[in] utf8_text UTF-8 text string will be displayed next time.
  ///
  void SetText(const std::string& utf8_text);

  ///
  /// @brief
  /// Update display albedo color which will be displayed on screen next render
  /// frame if only component and bound object are activated.
  ///
  /// @param[in] color Color values range from 0 to 1.
  /// If color value is out of range, clamped into 0 to 1.
  ///
  void SetColor(const glm::vec3& color);

  ///
  /// @brief
  ///
  ///
  /// @param[in] projection_matrix
  ///
  void SetProjectionMatrix(const glm::mat4& projection_matrix);

private:
  ///
  /// @brief
  /// Refresh m_text_container and m_text_render_width container.
  ///
  /// @param[in] text
  ///
  void RefreshStringContainers(const std::string& text);

  //! ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
  //! Member data.
  //! ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

  glm::mat4 m_projection = glm::mat4();
  GLuint m_vao = 0;
  GLuint m_vbo = 0;

  std::vector<uint32_t> m_text_render_width;
  Utf16TextContainer m_unicode_text_container;

  element::CShaderWrapper m_wrapper;
  manager::font::font_raw	m_font_set = nullptr;

  _internal::EDirtyFlag m_string_dirty = _internal::EDirtyFlag::Clean;
  _internal::EDirtyFlag m_color_dirty = _internal::EDirtyFlag::Dirty;
  _internal::EDirtyFlag m_proj_matrix_dirty = _internal::EDirtyFlag::Dirty;

  /// Color value range from 0 to 1, RGB support.
  glm::vec3 m_color;

  std::string m_font_name;

  std::string m_temporary_utf8_string;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CFont2DRenderer)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_FONT2D_RENDERER_H