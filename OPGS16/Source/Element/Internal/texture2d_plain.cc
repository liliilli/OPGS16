#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture2d_plain.cc
///
/// @author Jongmin Yun
///

/// Header file
#include <Element/Internal/texture2d_plain.h>
#include <Phitos/Dbg/assert.h>

namespace {

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

namespace opgs16::element::texture {

CTexture2DPlain::~CTexture2DPlain() {
  if (m_texture_id) {
    glDeleteTextures(1, &m_texture_id);
  }
}

bool CTexture2DPlain::Initialize(GLint internal_format,
                                 GLenum format, GLenum type,
                                 GLsizei width, GLsizei height) {
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
  return true;
}

} /// ::opgs16::element::texture namespace
