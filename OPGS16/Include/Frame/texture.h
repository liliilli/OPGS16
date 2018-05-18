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
 * 2018-04-06 IndexSize to unsigned value because fragmentation mechanism has been changed.
 * 2018-04-06 Change constructor parameter type STexture2D to STexture2DAtlas.
 */

#include <vector>       /*! std::vector */

#include <GL\glew.h>    /*! GL specific types */

/// ::opgs16::element::texture::_internal
#include <Element\Internal\texture_internal.h>

#include <Manager\resource_type.h>
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
/*!
 * @namespace m_texture
 * @brief Namespace for m_texture resource class.
 */
namespace texture {

/*!
 * @class CTexture2D
 * @brief The class for binding Texture id and can set m_texture's parameters.
 *
 * @log
 * 2018-02-23 m_texture::Texture2D Refactoring
 * 2018-02-28 Add Width, Height inline functions.
 * 2018-03-10 Refactoring.
 * 2018-04-06 Change constructor parameter type STexture2D to STexture2DAtlas.
 */
class CTexture2D {
    using TextureParameter = element::texture::_internal::TextureParameter;
    using cell_size = std::pair<float, float>;
public:
    enum class ETexelType { LEFT_DOWN, RIGHT_UP };

public:
    ~CTexture2D();

	/**
	 * @brief Constructor get path and m_texture mode as arguments, build and set them.
     * @param[in] container Container which stores texture atlas information.
	 */
	CTexture2D(const resource::STexture2DAtlas& container);

	/**
	 * @brief Create m_texture with no m_texture path, but for later use.
	 * @param[in] internal_format color format or internal format to set up each pixel.
	 * @param[in] format format of entire texture.
	 * @param[in] type type.
	 * @param[in] width texture width size to create.
	 * @param[in] height texture height size to create.
	 */
	CTexture2D(const GLint internal_format, GLenum format, GLenum type, GLsizei width = 256, GLsizei height = 224);

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
	GLuint Id() const noexcept {
        return m_texture;
    }

    /*! Get Texture overall width */
    int Width() const noexcept {
        return m_width;
    }

    /*! Get Texture overall height */
    int Height() const noexcept {
        return m_height;
    }

    /*!
     * @brief Get Texel information from m_texels with given index number.
     * If given index number is out of bound, return nullptr.
     * And is_atlas is false so this texture is not concerned with atlas, return nullptr;
     */
    const float* GetTexelPtr(const ETexelType texel_type, const unsigned index) const noexcept {
        if (m_texels && is_atlas && index < m_texels->size()) {
            switch (texel_type) {
            case ETexelType::LEFT_DOWN: return (*m_texels)[index].left_down;
            case ETexelType::RIGHT_UP:  return (*m_texels)[index].right_up;
            }
        }
        return nullptr;
	}

    /*! Return whether this texture handles atlas information or not. */
    bool DoesHasAtlas() const noexcept {
        return is_atlas;
	}

private:
	mutable GLuint m_texture{ 0 };      /*! Texture binding id */
    int m_width{ 0 };                   /*! Texture width */
    int m_height{ 0 };                  /*! Texture height */
    bool is_atlas{ false };

    const std::vector<resource::STexture2DTexelInformation>* m_texels{ nullptr };
};

} /*! opgs16::texture */
} /*! opgs16 */

#endif // OPGS16_SYSTEM_FRAME_TEXTURE_H