#ifndef OPGS16_SYSTEM_FRAME_TEXTURE_H
#define OPGS16_SYSTEM_FRAME_TEXTURE_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Frame/m_texture.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-23 m_texture::Texture2D Refactoring
 * 2018-02-28 Add Width, Height inline functions.
 * 2018-03-10 Refactoring.
 */

#include <vector>       /*! std::vector */
#include <GL/glew.h>    /*! GL specific types */
#include "../Element/Internal/texture_internal.h"   /*! ::opgs16::element::texture::_internal */
#include "../Manager/Public/resource_type.h"        /*! ::opgs16::resource */
#include "../../Headers/Fwd/objectfwd.h"            /*! ::opgs16::resource::Texture2D structure */

namespace opgs16 {
/*!
 * @namespace m_texture
 * @brief Namespace for m_texture resource class.
 */
namespace texture {

/*!
 * @class Texture2D
 * @brief The class for binding Texture id and can set m_texture's parameters.
 *
 * @log
 * 2018-02-23 m_texture::Texture2D Refactoring
 * 2018-02-28 Add Width, Height inline functions.
 * 2018-03-10 Refactoring.
 */
class Texture2D {
private:
    using TextureParameter = element::texture::_internal::TextureParameter;
    using cell_size = std::pair<float, float>;

public:
    ~Texture2D();

	/**
	 * @brief Constructor get path and m_texture mode as arguments, build and set them.
	 * @param[in] texture_path path to get m_texture data.
	 * @param[in] bind_mode mode to bind m_texture as what m_texture's color data type is.
	 * bind_mode is limited in GL_RGB, GL_RGBA, and so on.
	 */
	Texture2D(const resource::Texture2D& container);

	/**
	 * @brief Create m_texture with no m_texture path, but for later use.
	 * @param[in] internal_format color format or internal format to set up each pixel.
	 * @param[in] format format of entire texture.
	 * @param[in] type type.
	 * @param[in] width texture width size to create.
	 * @param[in] height texture height size to create.
	 */
	Texture2D(const GLint internal_format, GLenum format, GLenum type,
              GLsizei width = 256, GLsizei height = 224);

	/**
	 * @brief Set texture's rendering option. (one item version)
	 * @param[in] option Option of texture parameter, such as GL_TEXTURE_MIN_FILTER, etc
	 * @param[in] mode Mode for option of texture parameter, like a GL_NEAREST, GL_REPEAT
	 */
	void SetTextureParameterI(const GLint option, const GLint mode);

	/**
	 * @brief Set texture's rendering options. (multiple item version)
	 * @param[in] THe list of texture parameters.
	 */
	void SetTextureParameterI(const std::vector<TextureParameter>& lists);

	/**
	 * @brief Set border color of texture.
	 * @param[in] border_color array of border color which consists of R, G, B, A.
	 */
	void SetBorderColor(const std::array<GLfloat, 4>& border_color);

	/*! Get Texture id */
	inline GLuint Id() const {
        return m_texture;
    }

    /*! Get Texture overall width */
    inline int Width() const noexcept {
        return m_width;
    }

    /*! Get Texture overall height */
    inline int Height() const noexcept {
        return m_height;
    }

    /*! Get Texture cell size ranges [0, 1] */
    inline cell_size CellSize() const noexcept {
        return m_texture_cell_size;
    }

    inline resource::Texture2D::IndexSize CellWH() const noexcept {
        return m_cell_number;
	}

private:
	mutable GLuint m_texture;           /*! Texture binding id */

    int m_width;                        /*! Texture width */
    int m_height;                       /*! Texture height */

    cell_size m_texture_cell_size{};    /*! Texture coordinate cell size */
    resource::Texture2D::IndexSize m_cell_number{};
};

} /*! opgs16::texture */
} /*! opgs16 */

#endif // OPGS16_SYSTEM_FRAME_TEXTURE_H