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
 * @file System/Frame/Private/texture.cc
 * @brief Texture2D implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Move it to opgs16::texture namespace.
 * 2018-04-06 Change constructor parameter type STexture2D to STexture2DAtlas.
 */

#include "texture.h"                            /*! Header file */

#include <array>
#include "../../__ThirdParty/stb/stb_image.h"
#include "../Manager/Public/resource_type.h"    /*! resource::STexture2D */
#include "../../Headers/import_logger.h"        /*! import logger */

namespace opgs16 {
namespace texture {
namespace {

/*!
 * @brief Return color format
 * @param[in] channels Color channels value for being used to get GL_COLOR channels.
 */
GLenum ColorFormat(const int channels) noexcept {
    switch (channels) {
    default: return GL_NONE;    /*! else, return Error type */
    case 1: return GL_RED;      /*! Gray or Red (one channel) */
    case 3: return GL_RGB;      /*! RGB no alpha */
    case 4: return GL_RGBA;     /*! RGB and alpha */
    }
}

/*!
 * @brief Set texture default parameter setting.
 * GL_TEXTURE_MIN_FILTER to GL_NEAREST
 * GL_TEXTURE_MAG_FILTER to GL_NEAREST
 * GL_TEXTURE_WRAP_S to GL_REPEAT
 * GL_TEXTURE_WRAP_T to GL_REPEAT
 */
void SetDefaultParameterSetting(CTexture2D& texture) {
    using element::texture::_internal::TextureParameter;
    std::vector<TextureParameter> t_p;  /*! Default Texture parameters */
    t_p.push_back(TextureParameter{ GL_TEXTURE_MIN_FILTER, GL_NEAREST });
    t_p.push_back(TextureParameter{ GL_TEXTURE_MAG_FILTER, GL_NEAREST });
    t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_S, GL_REPEAT });
    t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_T, GL_REPEAT });
    texture.SetTextureParameterI(t_p);
}

} /*! unnamed namespace */

CTexture2D::~CTexture2D() {
    if (m_texture) glDeleteTextures(1, &m_texture);
}

CTexture2D::CTexture2D(const resource::STexture2DAtlas& container) {
    stbi_set_flip_vertically_on_load(true);
    auto nr_channels = 0;
    auto data = stbi_load(container.path.c_str(), &m_width, &m_height, &nr_channels, 0);
    if (data) {
        /*! Make m_texture */
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        const auto fmt = ColorFormat(nr_channels);
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_width, m_height, 0, fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        SetDefaultParameterSetting(*this);

        if (container.has_atlas) {
            is_atlas = true;
            m_texels = &container.texels;
        }
    }
    else {
        PUSH_LOG_ERRO(L"Failed load textrue, ");
    }

    stbi_image_free(data);
}

CTexture2D::CTexture2D(const GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height) {
    /*! Temporary */
    m_width = 256;
    m_height = 224;

    /*! Make Texture for entire screen size */
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, type, 0);
    SetDefaultParameterSetting(*this);
}

void CTexture2D::SetTextureParameterI(const GLint option, const GLint mode) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
}

void CTexture2D::SetTextureParameterI(const std::vector<TextureParameter>& lists) {

    glBindTexture(GL_TEXTURE_2D, m_texture);
    for (const auto& option_item : lists) {
        glTexParameteri(GL_TEXTURE_2D, option_item.option, option_item.mode);
    }
}

void CTexture2D::SetBorderColor(const std::array<GLfloat, 4>& border_color) {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);
}

///**
// * @brief   Generate cubemap m_texture.
// * @details
// * @param   texture_path m_texture path to load as cubemap m_texture.<br>
// * m_texture path should be "{path}/file_name[_dir].type", example "resources/sky.jpg".
// * @return  GLuint cubemap m_texture id
// *
// * @see     https://learnopengl.com/#!Model-Loading/Model
// */
//GLuint LoadCubemap(std::string texture_path) {
//    const std::array<std::string, 6> cubemap_paths = { "_rf", "_lf", "_tp", "_bt", "_bk", "_ft" };
//
//    // Create t_p cubemap
//    GLuint texture_cubemap;
//    glGenTextures(1, &texture_cubemap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);
//
//    // Separate texture_path to file_name and file_type
//    std::string file_name = texture_path.substr(0, texture_path.find_last_of('.'));
//    std::string file_type = texture_path.substr(texture_path.find_last_of('.'));
//
//    for (size_t i = 0; i < cubemap_paths.size(); ++i) {
//        int width, height, channels;
//        std::string file_path = file_name + cubemap_paths[i] + file_type;
//        auto* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
//
//        if (data) {
//            GLenum format;
//            switch (channels) {
//            case 1: format = GL_RED;    break;
//            case 3: format = GL_RGB;    break;
//            case 4: format = GL_RGBA;   break;
//            }
//
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
//                         GL_UNSIGNED_BYTE, data);
//        }
//        else {
//            std::cout << "Cubemap m_texture failed to load at path: " << std::endl;
//            throw std::runtime_error("Failed to create cubemap m_texture" + file_path);
//        }
//
//        stbi_image_free(data);
//    }
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//    return texture_cubemap;
//}

} /*! opgs16::texture */
} /*! opgs16 */