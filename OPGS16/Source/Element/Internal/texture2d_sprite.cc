#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture_2d.cc
///
/// @brief Texture2D implementation file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-10 Move it to opgs16::texture namespace.
/// 2018-04-06 Change constructor parameter type STexture2D to STexture2DAtlas.
///

/// Header file
#include <Element/Internal/texture2d_sprite.h>

#include <array>

#include <../__ThirdParty/stb/stb_image.h>

#include <Manager/resource_type.h>
#include <Headers/import_logger.h>

namespace {

///
/// @brief Return color format
/// @param[in] channels Color channels value for being used to get GL_COLOR channels.
///
GLenum ColorFormat(int32_t channels) noexcept {
  switch (channels) {
  default: return GL_NONE;    /*! else, return Error type */
  case 1: return GL_RED;      /*! Gray or Red (one channel) */
  case 3: return GL_RGB;      /*! RGB no alpha */
  case 4: return GL_RGBA;     /*! RGB and alpha */
  }
}

///
/// @brief Set texture default parameter setting.
/// GL_TEXTURE_MIN_FILTER to GL_NEAREST
/// GL_TEXTURE_MAG_FILTER to GL_NEAREST
/// GL_TEXTURE_WRAP_S to GL_REPEAT
/// GL_TEXTURE_WRAP_T to GL_REPEAT
///
void SetDefaultParameterSetting(opgs16::texture::CTexture2DSprite& texture) {
  using opgs16::element::texture::_internal::DTextureParameter;
  // Default Texture parameters.
  std::vector<DTextureParameter> t_p;

  t_p.push_back(DTextureParameter{ GL_TEXTURE_MIN_FILTER, GL_NEAREST });
  t_p.push_back(DTextureParameter{ GL_TEXTURE_MAG_FILTER, GL_NEAREST });
  t_p.push_back(DTextureParameter{ GL_TEXTURE_WRAP_S, GL_REPEAT });
  t_p.push_back(DTextureParameter{ GL_TEXTURE_WRAP_T, GL_REPEAT });
  texture.SetTextureParameterI(t_p);
}

} /// unnamed namespace

namespace opgs16::texture {

CTexture2DSprite::~CTexture2DSprite() {
  if (m_texture_id) {
    glDeleteTextures(1, &m_texture_id);
  };
}

CTexture2DSprite::CTexture2DSprite(const resource::STexture2DAtlas& container) {
  stbi_set_flip_vertically_on_load(true);
  auto nr_channels = 0;
  auto data = stbi_load(container.path.c_str(), &m_texture_width, &m_texture_height, &nr_channels, 0);

  if (data) {
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    const auto fmt = ColorFormat(nr_channels);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt,
                 m_texture_width, m_texture_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 data);

    //glGenerateMipmap(GL_TEXTURE_2D);
    SetDefaultParameterSetting(*this);

    if (container.has_atlas) {
      is_texture_atlas = true;
      m_texels = &container.texels;
    }
  }
  else {
    PUSH_LOG_ERROR_EXT("Failed load textrue. [Name : {}] [Path : {}].", container.name, container.path);
  }

  stbi_image_free(data);
}

CTexture2DSprite::CTexture2DSprite(const GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height) {
  PHITOS_ASSERT(width > 0,  "Width value must be bigger than 0.");
  PHITOS_ASSERT(height > 0, "Height value must be bigger than 0.");
  m_texture_width = width; m_texture_height = height;

  // Make Texture for entire screen size.
  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0,
               internal_format, width, height,
               0, format, type, nullptr);
  SetDefaultParameterSetting(*this);
}

void CTexture2DSprite::SetTextureParameterI(const GLint option, const GLint mode) {
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexParameteri(GL_TEXTURE_2D, option, mode);
}

void CTexture2DSprite::SetTextureParameterI(const std::vector<TextureParameter>& lists) {
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  for (const auto& option_item : lists) {
    glTexParameteri(GL_TEXTURE_2D, option_item.option, option_item.mode);
  }
}

void CTexture2DSprite::SetBorderColor(const std::array<GLfloat, 4>& border_color) {
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);
}

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

} /*! opgs16::texture */