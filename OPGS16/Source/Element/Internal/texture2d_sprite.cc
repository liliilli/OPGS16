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

#include <../__ThirdParty/stb/stb_image.h>
#include <Headers/import_logger.h>

namespace {

///
/// @brief Return color format
/// @param[in] channels Color channels value for being used to get GL_COLOR channels.
///
GLenum ColorFormat(int32_t channels) noexcept {
  switch (channels) {
  case 1:   return GL_RED;      /// Gray or Red (one channel)
  case 3:   return GL_RGB;      /// RGB no alpha
  case 4:   return GL_RGBA;     /// RGB and alpha
  default:  return GL_NONE;     /// else, return Error type
  }
}

///
/// @brief Set texture default parameter setting.
/// GL_TEXTURE_MIN_FILTER to GL_NEAREST
/// GL_TEXTURE_MAG_FILTER to GL_NEAREST
/// GL_TEXTURE_WRAP_S to GL_REPEAT
/// GL_TEXTURE_WRAP_T to GL_REPEAT
///
void SetDefaultParameterSetting(opgs16::element::_internal::CTextureBase& texture) {
  using opgs16::element::_internal::DTextureParameter;
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
  }
}

bool CTexture2DSprite::Initialize(const resource::STexture2DAtlas& container) {
  bool result = false;
  auto nr_channels = 0;

  stbi_set_flip_vertically_on_load(true);
  const auto data = stbi_load(container.path.c_str(),
                              &m_texture_width, &m_texture_height,
                              &nr_channels, 0);
  if (data) {
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    const auto fmt = ColorFormat(nr_channels);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt,
                 m_texture_width, m_texture_height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 data);
    SetDefaultParameterSetting(*this);

    if (container.has_atlas) {
      is_texture_atlas = true;
      m_texels = &container.texels;
    }

    m_is_own_resource = true;
    result = true;
  }
  else {
    PUSH_LOG_ERROR_EXT("Failed load textrue. [Name : {}] [Path : {}].",
                       container.name, container.path);
  }

  stbi_image_free(data);
  return result;
}

std::optional<DVector2>
CTexture2DSprite::GetTexelPtr(ETexelType texel_type, int32_t index) const noexcept {
  if (m_texels && is_texture_atlas && index < m_texels->size()) {
    switch (texel_type) {
    case ETexelType::LeftDown: {
      const float(&xy)[2] = (*m_texels)[index].left_down;
      return DVector2{ xy[0], xy[1] };
    }
    case ETexelType::RightUp: {
      const float(&xy)[2] = (*m_texels)[index].right_up;
      return DVector2{ xy[0], xy[1] };
    }
    }
  }

  return std::nullopt;
}

} /*! opgs16::texture */