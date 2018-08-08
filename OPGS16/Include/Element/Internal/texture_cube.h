#ifndef OPGS16_ELEMENT_INTERNAL_TEXTURE_CUBE_H
#define OPGS16_ELEMENT_INTERNAL_TEXTURE_CUBE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture_cube.h
///
/// @author Jongmin Yun
///

#include <Element/Internal/texture_base.h>

namespace opgs16::element::_internal {

class CTextureCubemap final : public _internal::CTextureBase {
public:

  ///**
// * @brief   Generate cubemap m_texture_id.
// * @details
// * @param   texture_path m_texture_id path to load as cubemap m_texture_id.<br>
// * m_texture_id path should be "{path}/file_name[_dir].type", example "resources/sky.jpg".
// * @return  GLuint cubemap m_texture_id id
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
//            std::cout << "Cubemap m_texture_id failed to load at path: " << std::endl;
//            throw std::runtime_error("Failed to create cubemap m_texture_id" + file_path);
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

private:

};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_TEXTURE_CUBE_H