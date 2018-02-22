#include "texture.h"

#include <array>
#include <iostream>
#include "..\..\stb_image.h"

namespace texture {

Texture2D::Texture2D(const std::string& texture_path) {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

	stbi_set_flip_vertically_on_load(true);
    /*!
     * Set the m_texture wrapping and filtering options
     * Load image file to use as m_texture image. */

    auto nr_channels = 0;
    auto data = stbi_load(texture_path.c_str(), &m_width, &m_height, &nr_channels, 0);
    if (data) {
        GLenum color_format;
        switch (nr_channels) {
            case 1: color_format = GL_RED;    break;    /*! Gray or Red (one channel) */
            case 3: color_format = GL_RGB;    break;    /*! RGB no alpha */
            case 4: color_format = GL_RGBA;   break;    /*! RGB and alpha */
        }

        /*! Make m_texture */
        glTexImage2D(GL_TEXTURE_2D, 0, color_format,
                     m_width, m_height, 0, color_format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /*! Setting default option */
		std::vector<TextureParameter> t_p;  /*! Default Texture parameters */
		t_p.push_back(TextureParameter{ GL_TEXTURE_MIN_FILTER, GL_NEAREST });
		t_p.push_back(TextureParameter{ GL_TEXTURE_MAG_FILTER, GL_NEAREST });
		t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_S, GL_REPEAT });
		t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_T, GL_REPEAT });
		SetTextureParameterI(t_p);
    }
    else {
        std::cerr << "FAILED::LOAD::TEXTURE" + texture_path << std::endl;
        throw std::runtime_error{ "Failed load texture. " "Texture path is " + texture_path };
    }

    /*! Release image buffer */
    stbi_image_free(data);
}

Texture2D::Texture2D(const GLint internal_format,
                     GLenum format, GLenum type, GLsizei width, GLsizei height) {
    /*! Temporary */
    m_width     = 256;
    m_height    = 224;

    /*! Make Texture for entire screen size */
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, type, 0);

    /*! Setting default option */
	std::vector<TextureParameter> t_p;  /*! Default Texture parameters */
	t_p.push_back(TextureParameter{ GL_TEXTURE_MIN_FILTER, GL_NEAREST });
	t_p.push_back(TextureParameter{ GL_TEXTURE_MAG_FILTER, GL_NEAREST });
	t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_S, GL_REPEAT });
	t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_T, GL_REPEAT });
	SetTextureParameterI(t_p);
}

void Texture2D::SetTextureParameterI(const GLint option, const GLint mode) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
}


void Texture2D::SetTextureParameterI(const std::vector<TextureParameter>& lists) {

    glBindTexture(GL_TEXTURE_2D, m_texture);
    for (const auto& option_item : lists) {
        glTexParameteri(GL_TEXTURE_2D, option_item.option, option_item.mode);
    }
}

void Texture2D::SetBorderColor(const std::array<GLfloat, 4>& border_color) {
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

}
