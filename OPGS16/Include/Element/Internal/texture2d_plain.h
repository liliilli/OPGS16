#ifndef OPGS16_ELEMENT_INTERNAL_TEXTURE2D_PLAIN_H
#define OPGS16_ELEMENT_INTERNAL_TEXTURE2D_PLAIN_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture2d_plain.h
///
/// @author Jongmin Yun
///

#include <Element/Internal/texture_base.h>

namespace opgs16::element::texture {

///
/// @class CTexture2DPlain
/// @brief
///
class CTexture2DPlain final : public _internal::CTextureBase {
public:
  ~CTexture2DPlain();

	///
	/// @brief Create m_texture with no m_texture path, but for later use.
	/// @param[in] internal_format color format or internal format to set up each pixel.
	/// @param[in] format format of entire texture.
	/// @param[in] type type.
	/// @param[in] width texture width size to create.
	/// @param[in] height texture height size to create.
	///
  bool Initialize(GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height);

private:

};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_INTERNAL_TEXTURE2D_PLAIN_H