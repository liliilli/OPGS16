#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/texture_base.cc
///
/// @author Jongmin Yun
///

/// Header file
#include <Element/Internal/texture_base.h>

#include <Element/Internal/texture_internal.h>
#include <Headers/import_logger.h>

namespace opgs16::element::_internal {

void CTextureBase::SetTextureParameterI(const GLint option, const GLint mode) {
  if (m_texture_id) {
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, option, mode);
  }
}

void CTextureBase::SetTextureParameterI(const std::vector<DTextureParameter>& lists) {
  if (m_texture_id) {
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    for (const auto& option_item : lists) {
      glTexParameteri(GL_TEXTURE_2D, option_item.option, option_item.mode);
    }
  }
}

void CTextureBase::SetBorderColor(const std::array<GLfloat, 4>& border_color) {
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color[0]);
}

} /// ::opgs16::element::_internal namespace