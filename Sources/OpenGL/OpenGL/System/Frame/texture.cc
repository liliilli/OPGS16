#include "texture.h"

#include <array>
#include <iostream>
#include "..\..\stb_image.h"

namespace texture {

Texture2D::Texture2D(const std::string& texture_path, const GLint bind_mode) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
    // Set the texture wrapping and filtering options
    // Load image file to use as texture image.
    auto width = 0;
    auto height = 0;
    auto nr_channels = 0;
    auto data = stbi_load(texture_path.c_str(), &width, &height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, bind_mode, width, height, 0,
                     bind_mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "FAILED::LOAD::TEXTURE" + texture_path << std::endl;
        throw std::runtime_error{ "Failed load texture. " "Texture path is " + texture_path };
    }

    stbi_image_free(data);
}

void Texture2D::SetTextureParameterI(const GLint option, const GLint mode) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
}


void Texture2D::SetTextureParameterI
(std::initializer_list<TextureParameter> lists) {

    glBindTexture(GL_TEXTURE_2D, texture);
    for (const auto& option_item : lists) {
        glTexParameteri(GL_TEXTURE_2D, option_item.option, option_item.mode);
    }
}

GLuint LoadCubemap(std::string texture_path) {
    const std::array<std::string, 6> cubemap_paths = { "_rf", "_lf", "_tp", "_bt", "_bk", "_ft" };

    // Create a cubemap
    GLuint texture_cubemap;
    glGenTextures(1, &texture_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

    // Separate texture_path to file_name and file_type
    std::string file_name = texture_path.substr(0, texture_path.find_last_of('.'));
    std::string file_type = texture_path.substr(texture_path.find_last_of('.'));

    for (size_t i = 0; i < cubemap_paths.size(); ++i) {
        int width, height, channels;
        std::string file_path = file_name + cubemap_paths[i] + file_type;
        auto* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

        if (data) {
            GLenum format;
            switch (channels) {
            case 1: format = GL_RED;    break;
            case 3: format = GL_RGB;    break;
            case 4: format = GL_RGBA;   break;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << std::endl;
            throw std::runtime_error("Failed to create cubemap texture" + file_path);
        }

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture_cubemap;
}

}
