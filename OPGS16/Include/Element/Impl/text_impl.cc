/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file GlobalObjects/Canvas/Impl/text_impl.cc
 * @brief Definition functions not inlined of ./text_impl.h
 * @log
 * 2018-04-17 Add comments and move definition functions to ::opgs16::element::canvas namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\Impl\text_impl.h> /// Header file

#include <Element\Canvas\text.h>    /// ::opgs16::element::canvas::CText
#include <Manager\font_manager.h>   /// ::opgs16::manager::MFontManager

namespace opgs16 {
namespace element {
namespace canvas {
namespace _internal {
CTextImpl::CTextImpl(CText& handle_ref) : m_handle_ref{ handle_ref } {}

void CTextImpl::SetFontSize(const unsigned size) {
    unsigned def = manager::k_default_font_size;
    m_handle_ref.SetScaleValue(static_cast<float>(size) / static_cast<float>(def));
}

void CTextImpl::SetColor(const glm::vec3& color) {
    m_color = color;
}

void CTextImpl::SetColor(const float r, const float g, const float b) {
    m_color = glm::vec3{ r, g, b };
}

} /*! opgs16::element::canvas::_internal */
} /*! opgs16::element::canvas */
} /*! opgs16::element */
} /*! opgs16 */