#ifndef OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H
#define OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture_internal.h
///
/// @brief ::opgs16::element::texture internal type implementation file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-11 Create file
/// 2018-08-08 Rename TextureParameter to DTextureParameter.
///

#include <GL/glew.h>

namespace opgs16::element::_internal {

///
/// @struct DTextureParameter
/// @brief Inner struct used in class Texture2D methods, SetTextureParameterI.
///
struct DTextureParameter final {
  /// Option of m_texture parameter, such as GL_TEXTURE_MIN_FILTER, etc.
  GLint option;
  /// Mode of m_texture parameter option, like a GL_NEAREST, GL_REPEAT.
  GLint mode;
};

} /// ::opgs16::element::texture::_internal namespace

#endif /// !OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H


