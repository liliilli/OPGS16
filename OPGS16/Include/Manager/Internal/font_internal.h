#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_FONT_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_FONT_INTERNAL_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/font_internal.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Create file, move MFontManager::DCharacter to this file.
/// 2018-06-20 Renovate DCharacter resource structure.
///

#include <glm/vec2.hpp>
#include <GL/glew.h>

namespace opgs16::manager::_internal {

///
/// @struct DCharacter
///
/// @brief Manages each texture of font glyphes.
///
struct DCharacter {
  /// texture id of each character.
  mutable uint32_t texture_id;

  glm::ivec2  size;		  /// glyph size
  glm::ivec2  bearing;	/// glyph bearing position
  uint32_t    advance;	/// glyph advance width

  explicit DCharacter(
      const unsigned texture_id, const glm::ivec2 size,
      const glm::ivec2 bearing, const unsigned advance) :
      texture_id{ texture_id },
      size{ size },
      bearing{ bearing },
      advance{ advance } {};

  /// Default constructor for unordered map
  DCharacter() : texture_id(unsigned{}), advance(unsigned{}) {};

  DCharacter(const DCharacter& rhs) noexcept :
      texture_id{ rhs.texture_id },
      size { rhs.size },
      bearing { rhs.bearing },
      advance{ rhs.advance } {
    rhs.texture_id = 0;
  }

  DCharacter& operator=(const DCharacter& rhs) noexcept {
    texture_id = rhs.texture_id;
    size = rhs.size;
    bearing = rhs.bearing;
    advance = rhs.advance;
    rhs.texture_id = 0;

    return *this;
  }

  ~DCharacter() {
    if (texture_id) {
      glDeleteTextures(1, &texture_id);
    }
  }
};

} /// opgs16::manager::_internal

#endif /// !OPGS16_SYSTEM_MANAGER_INTERNAL_FONT_INTERNAL_H

