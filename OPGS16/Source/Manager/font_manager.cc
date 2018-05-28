
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/font_manager.cc
///
/// @brief
/// This file manages font on memory, font information, font glyphs.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring.
/// 2018-05-26 Remove singleton and replace it with namespace at first.
///
/// @todo Implement Non-Ascii characters.
///

#include <Manager/font_manager.h>     // Header file.

#include <iostream>
#include <functional>

#include <ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H

/// opgs16::manager::MResourceManager
#include <Manager/resource_manager.h>
/// opgs16::manager::ShaderManager
#include <Manager/shader_manager.h>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Forward Declaration
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// @brief Checks freetype pointer with FontPath.
///
/// @return If checking is successful, return true. otherwise return false.
///
bool CheckFreeType() noexcept;

///
/// @brief
///
/// @param[in] font_path
///
bool LoadFreeType(const std::string& font_path) noexcept;

///
/// @brief
/// The method sets character textures from glyphs and store them to container.
/// This methods called when initiate instance.
///
/// @return Created font glyph container unique_ptr (moved)
///
opgs16::manager::font::font_map_ptr GetCharTextures();

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {

/// Default font size variable
constexpr unsigned k_default_font_size = 16u;

// Freetype pointer

/// Freetype library pointer
FT_Library	m_freetype{ nullptr };
/// Freetype face pointer used when initiating fonts.
FT_Face		m_ft_face{ nullptr };

// Restrict first 128 characters for now.

/// Container stores fonts.
std::unordered_map<std::string, opgs16::manager::font::font_map_ptr> m_fonts{};
opgs16::manager::font::font_raw	m_default_font{ nullptr };

opgs16::element::CShaderNew* m_shader{};

} /// unnamed namespace

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {

} /// unnamed namespace

namespace opgs16::manager::font {

void Initiate() {
	m_shader = ShaderManager::Instance().Shader("gCommonFont");
}

bool GenerateFont(const std::string& name_tag) {
  if (IsFontExist(name_tag))
    return false;

  const auto&[success, information] = resource::GetFont(name_tag);

  if (success && CheckFreeType() && LoadFreeType(information->Path())) {
    FT_Set_Pixel_Sizes(m_ft_face, 0, k_default_font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create font and move it.
    m_fonts.emplace(name_tag, GetCharTextures());
    // If caller order this font must be a default, insert raw pointer.
    if (information->IsDefault())
      m_default_font = m_fonts.at(name_tag).get();

    FT_Done_Face(m_ft_face);
    FT_Done_FreeType(m_freetype);
    return true;
  }

  return false;
}

font_type* GetDefaultFont() {
  if (!m_default_font) {
    NEU_NOT_IMPLEMENTED_ASSERT();
  }

  return m_default_font;
}

  std::optional<font_type*> GetFontSetPtr(const std::string& font_name_tag) {
  if (!IsFontExist(font_name_tag))
    return std::nullopt;

  return m_fonts[font_name_tag].get();
}

bool DeleteFont(const std::string& tag) {
  if (IsFontExist(tag)) {
    // Remove pointer reference
    const auto font = m_fonts.at(tag).get();
    if (m_default_font == font) m_default_font = nullptr;
    m_fonts.erase(tag);
    return true;
  }
  return false;
}

bool IsFontExist(const std::string& font_tag) {
  return m_fonts.find(font_tag) != m_fonts.end();
}

unsigned GetDefaultFontSize() {
  return k_default_font_size;
}

} /// ::opgs16::manager::font

bool CheckFreeType() noexcept {
  // Check Freetype is well.
  if (FT_Init_FreeType(&m_freetype)) {
    std::cerr << "ERROR::FREETYPE: Could not init Freetype Library\n";
    return false;
  }

  return true;
}

bool LoadFreeType(const std::string& font_path) noexcept {
  if (FT_New_Face(m_freetype, font_path.c_str(), 0, &m_ft_face)) {
    std::cerr << "ERROR::FREETYPE: Failed to load font\n";
    return false;
  }

  return true;
}

opgs16::manager::font::font_map_ptr GetCharTextures() {
  auto glyphs = std::make_unique<opgs16::manager::font::font_type>();

  for (GLubyte c = 0; c < 128; ++c) {
    if (FT_Load_Char(m_ft_face, c, FT_LOAD_RENDER)) {
      std::cerr << "ERROR::FREETYPE: Failed to load Glyph\n";
      continue;
    }

    /*! Generate Texture */
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    const auto width = m_ft_face->glyph->bitmap.width;
    const auto height = m_ft_face->glyph->bitmap.rows;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
      width, height,
      0, GL_RED, GL_UNSIGNED_BYTE,
      m_ft_face->glyph->bitmap.buffer);

    // Set Texture Options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Store character for later use
    glyphs->emplace(c, opgs16::manager::_internal::Character{ texture,
                    glm::ivec2(width, height),
                    glm::ivec2(m_ft_face->glyph->bitmap_left,
                               m_ft_face->glyph->bitmap_top),
                    static_cast<GLuint>(m_ft_face->glyph->advance.x)
      });
  }

  return glyphs;
}
