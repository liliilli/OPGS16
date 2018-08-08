#ifndef OPGS16_ELEMENT_INTERNAL_TEXTURE_BASE_H
#define OPGS16_ELEMENT_INTERNAL_TEXTURE_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-08 Create file.
///

#include <GL/glew.h>

#include <Element/Internal/texture_internal.h>

namespace opgs16::element::_internal {

///
/// @class CTextureBase
/// @brief All of texture base class.
///
class CTextureBase {
public:
  CTextureBase() = default;
  CTextureBase(const CTextureBase&) = default;
  CTextureBase(CTextureBase&&) = default;
  CTextureBase& operator=(const CTextureBase&) = default;
  CTextureBase& operator=(CTextureBase&&) = default;
  virtual ~CTextureBase() = default;

	///
	/// @brief Set texture's rendering option. (one item version)
	/// @param[in] option Option of texture parameter, such as GL_TEXTURE_MIN_FILTER, etc
	/// @param[in] mode Mode for option of texture parameter, like a GL_NEAREST, GL_REPEAT
	///
	void SetTextureParameterI(const GLint option, const GLint mode);

	///
	/// @brief Set texture's rendering options. (multiple item version)
	/// @param[in] lists The list of texture parameters.
	///
	void SetTextureParameterI(const std::vector<DTextureParameter>& lists);

	///
	/// @brief Set border color of texture.
	/// @param[in] border_color array of border color which consists of R, G, B, A.
	///
	void SetBorderColor(const std::array<GLfloat, 4>& border_color);

	///
  /// @brief Get Texture id
  ///
	int32_t GetTextureId() const noexcept {
    return m_texture_id;
  }

  ///
  /// @brief Get Texture overall width
  ///
  int32_t GetTextureWidth() const noexcept {
    return m_texture_width;
  }

  ///
  /// @brief Get Texture overall height
  ///
  int32_t GetTextureHeight() const noexcept {
    return m_texture_height;
  }

protected:
  /// Valid texture id must not be 0.
	mutable GLuint m_texture_id = 0;
  int32_t m_texture_width  = 0;
  int32_t m_texture_height = 0;

  bool m_is_own_resource = false;
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_TEXTURE_BASE_H