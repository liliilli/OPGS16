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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Canvas/Private/text.cc
 * @brief Definition file of ../Public/text.h.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-15 Unknown.
 * 2018-04-17 Move definition function body into ::opgs16::element::canvas namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/text.h"             /*! Header file */
#include "../../Impl/text_impl.h"       /*! ::ogps16::element::canvas::_internal::CTextImpl */
#include "../../System/Components/Public/empty_renderer.h" /*! ::opgs16::component::CEmptyRenderer */
#include "../../System/Manager/Public/font_manager.h"  /*! MFontManager
                                                      * aliasing font_map_ptr
                                                      * FontManager& Instance()
                                                      * bool LoadDefaultFont()
                                                      * void RenderTextNew(params)
                                                      * bool DoesFontExist(std::string) */
#include "../../../Headers/import_logger.h" /*! import logger in debug mode */

namespace opgs16 {
namespace element {
namespace canvas {
namespace _internal {
void TextImplDeleter::operator()(CTextImpl* p) { delete p; }

} /*! opgs16::element::canvas::_internal */

using fontMap = manager::MFontManager::font_map_ptr;
using CTextImpl = _internal::CTextImpl;

CText::CText(const std::string& initial_txt, const glm::vec3& position, const glm::vec3& color) :
	m_font_manager{ &manager::MFontManager::Instance() } {
	/** Body */
    std::unique_ptr<CTextImpl, _internal::TextImplDeleter> impl{ new CTextImpl(*this) };
    m_text_impl = std::move(impl);
    m_text_impl->SetText(initial_txt);
    m_text_impl->SetColor(color);
	SetWorldPosition(position);

    AddComponent<component::CEmptyRenderer>(*this);
}

void CText::Render() {
	if (m_font_manager) {
		/** Set font */
        auto font_name = m_text_impl->GetFontName();
		if (font_name.empty())
            m_font_manager->LoadDefaultFont();
		else
            m_font_manager->LoadFont(font_name);

		/** Render */
		m_font_manager->RenderTextNew(m_text_impl->GetText(),
                                      GetOrigin(),
                                      glm::vec2{ GetFinalPosition() },
                                      m_text_impl->GetColor(),
                                      GetAlignment(),
                                      GetScaleValue());
	}
	else {
        PUSH_LOG_ERRO(L"Could not find font manager.");
	}
}

void CText::SetText(const std::string& new_text) {
    m_text_impl->SetText(new_text);
}

const std::string CText::GetText() const {
    return m_text_impl->GetText();
}

void CText::SetFontSize(const unsigned size) {
    m_text_impl->SetFontSize(size);
}

const unsigned CText::GetFontSize() const {
    return m_text_impl->GetFontSize();
}

bool CText::SetFontName(const std::string& font_tag) {
	if (m_font_manager && m_font_manager->DoesFontExist(font_tag)) {
		m_text_impl->SetFontName(font_tag);
		return true;
	}
	else {
        PUSH_LOG_ERRO(L"Font not found.");
		//std::cerr << "ERROR::FONT::NOT::FOUND" << font_tag << std::endl;
		m_text_impl->SetFontName("");
		return false;
	}
}

void CText::SetColor(const glm::vec3 & color) {
    m_text_impl->SetColor(color);
}

} /*! opgs16::element::canvas */
} /*! opgs16::element */
} /*! ogps16 */
