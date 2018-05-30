
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/font_manager.cc
///
/// @brief
/// This file manages font, font information, font glyphs.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring.
/// 2018-05-26 Remove singleton and replace it with namespace at first.
/// 2018-05-30 Implement Unicode characters except for surrgate characters.
///

/// Header file.
#include <Manager/font_manager.h>

#include <functional>

#include <ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H

/// ::phitos Expanded Assertion.
#include <Phitos/Dbg/assert.h>

/// opgs16::manager::MResourceManager
#include <Manager/resource_manager.h>
/// opgs16::manager::ShaderManager
#include <Manager/shader_manager.h>
/// ::opgs16::manager::_internal flags
#include <Manager/Internal/flag.h>

/// import logger header file
#include <Headers/import_logger.h>

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
opgs16::manager::font::font_map_ptr GetAsciiCharTextures();

///
/// @brief
///
/// @return
///
opgs16::manager::_internal::Character GetCharTexture(char16_t chr);

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;
EInitiated m_initiated = EInitiated::NotInitiated;
} /// unnamed namespace

///
/// This namespace stores variables or
/// constexpr variables to be used by functions.
///
namespace {
/// Default font size variable
constexpr unsigned k_default_font_size = 64u;

//!
//! Freetype pointer
//!

/// Freetype library pointer
FT_Library	m_freetype = nullptr;
/// Freetype face pointer used when initiating fonts.
FT_Face		  m_ft_face = nullptr;

//!
//! Members
//!

opgs16::element::CShaderNew* m_common_shader = nullptr;
opgs16::manager::font::font_raw	m_default_font = nullptr;

/// Container which stores fonts.
std::unordered_map<std::string, opgs16::manager::font::font_map_ptr> m_fonts{};

} /// unnamed namespace

namespace opgs16::manager::font {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
      "Duplicated function call of ::opgs16::manager::font::Initiate() is prohibited.");
	m_common_shader = ShaderManager::Instance().Shader("gCommonFont");
}

bool GenerateFont(const std::string& name_tag) {
  if (IsFontExist(name_tag)) {
    PUSH_LOG_WARN_EXT("Font already has been. : [font : {}]", name_tag);
    return false;
  }

  if (const auto& [success, information] = resource::GetFont(name_tag);
      success && CheckFreeType() && LoadFreeType(information->Path())) {
    FT_Set_Pixel_Sizes(m_ft_face, 0, k_default_font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create font and move it.
    auto [it, is_created] = m_fonts.emplace(name_tag, GetAsciiCharTextures());
    if (is_created) {
      PUSH_LOG_INFO_EXT("Font {0} has been created successfully.", name_tag);
    }
    else {
      PUSH_LOG_WARN_EXT(
          "Font {0} has not been created successfully."
          "There is already a font.",
          name_tag);
    }

    // If caller order this font must be a default, insert raw pointer.
    if (information->IsDefault())
      m_default_font = m_fonts[name_tag].get();

    FT_Done_Face(m_ft_face);
    FT_Done_FreeType(m_freetype);
    return true;
  }

  PUSH_LOG_ERROR_EXT("GenerateFont({}) was not succeeded properly.", name_tag);
  return false;
}

bool GenerateCharacter(const std::string& font_name, const char16_t utf16_char) {
  if (!IsFontExist(font_name)) {
    PHITOS_ASSERT(IsFontExist(font_name), "Font is not exist on runtime.");
    return false;
  }

  if (const auto&[success, information] = resource::GetFont(font_name);
      success && CheckFreeType() && LoadFreeType(information->Path())) {
    FT_Set_Pixel_Sizes(m_ft_face, 0, k_default_font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create character glyph.
    auto [it, is_created] =
        (*m_fonts[font_name]).emplace(utf16_char, GetCharTexture(utf16_char));
    if (!is_created) {
      PUSH_LOG_WARN_EXT(
          "Character {0} of font {1} has not been created successfully."
          "There is already a glyph or not.",
          static_cast<int>(utf16_char), font_name);
    }

    // If caller order this font must be a default, insert raw pointer.
    if (information->IsDefault())
      m_default_font = m_fonts[font_name].get();

    FT_Done_Face(m_ft_face);
    FT_Done_FreeType(m_freetype);
    return true;
  }

  PUSH_LOG_ERROR_EXT(
      "GenerateCharacter({}, {}) was not succeeded properly.",
      font_name, static_cast<int>(utf16_char));
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

bool DeleteFont(const std::string& font_name) {
  if (IsFontExist(font_name)) {
    // Remove pointer reference
    const auto font = m_fonts[font_name].get();
    if (m_default_font == font)
      m_default_font = nullptr;
    m_fonts.erase(font_name);

    PUSH_LOG_INFO_EXT("Font, {0} was deleted successfully.", font_name);
    return true;
  }

  PUSH_LOG_WARN_EXT("Font {0} is not exist.", font_name);
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
    PUSH_LOG_ERRO("Could not initiate Freetype font library.");
    return false;
  }

  return true;
}

bool LoadFreeType(const std::string& font_path) noexcept {
  if (FT_New_Face(m_freetype, font_path.c_str(), 0, &m_ft_face)) {
    PUSH_LOG_ERROR_EXT(
        "Coult not load font information : [path : {}]", font_path);
    return false;
  }

  return true;
}

opgs16::manager::font::font_map_ptr GetAsciiCharTextures() {
  auto glyphs = std::make_unique<opgs16::manager::font::font_type>();

  for (char16_t c = 0; c < 128; ++c) {
    const auto character = GetCharTexture(c);
    glyphs->emplace(c, character);
  }

  return glyphs;
}

opgs16::manager::_internal::Character GetCharTexture(char16_t chr) {
  if (FT_Load_Char(m_ft_face, chr, FT_LOAD_RENDER)) {
    PUSH_LOG_ERROR_EXT(
        "Failed to load character glyph. : [character : {0}]",
        static_cast<int>(chr));
    return {};
  }

  // Generate Textures
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  const auto width  = m_ft_face->glyph->bitmap.width;
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
  return opgs16::manager::_internal::Character{
      texture,
      glm::ivec2(width, height),
      glm::ivec2(m_ft_face->glyph->bitmap_left,
          m_ft_face->glyph->bitmap_top),
      static_cast<GLuint>(m_ft_face->glyph->advance.x)
  };
}
