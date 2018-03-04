#include "text_impl.h"  /** Header file */
#include "..\text.h"    /** Canvas::Text
                          * void SetScaleLocalValue(float)
                          * const FontManager& GetFontManager() */
#include "../../../System/Manager/Public/font_manager.h"

namespace canvas {
TextImpl::TextImpl(Text& handle_ref) : m_handle_ref{ handle_ref } {}

void TextImpl::SetFontSize(const unsigned size) {
    unsigned def = opgs16::manager::k_default_font_size;
	m_handle_ref.SetScaleValue(static_cast<float>(size) / static_cast<float>(def));
}

void TextImpl::SetColor(const glm::vec3& color) {
    m_color = color;
}

void TextImpl::SetColor(const float r, const float g, const float b) {
    m_color = glm::vec3{ r, g, b };
}

}