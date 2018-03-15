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

/*!
 * @file GlobalObjects/Canvas/text.cc
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-15
 */

#include "text.h"                       /*! Header file */
#include <iostream>                     /*! std::cerr
                                          * std::endl */
#include "Impl/text_impl.h"             /*! Canvas::TextImpl */
#include "../../System/Manager/Public/font_manager.h"  /*! MFontManager
                                                  * aliasing font_map_ptr
                                                  * FontManager& Instance()
                                                  * bool LoadDefaultFont()
                                                  * void RenderTextNew(params)
                                                  * bool DoesFontExist(std::string) */
/*! ::opgs16::component::CEmptyRenderer */
#include "../../System/Components/Public/empty_renderer.h"

namespace canvas {
using fontMap = opgs16::manager::MFontManager::font_map_ptr;
void TextImplDeleter::operator()(TextImpl* p) { delete p; }

Text::Text(const std::string& initial_txt, const glm::vec3& position, const glm::vec3& color) :
	m_font_manager{ &opgs16::manager::MFontManager::Instance() } {
	/** Body */
    std::unique_ptr<TextImpl, TextImplDeleter> impl{ new TextImpl(*this) };
    m_text_impl = std::move(impl);
    m_text_impl->SetText(initial_txt);
    m_text_impl->SetColor(color);
	SetWorldPosition(position);

    AddComponent<::opgs16::component::CEmptyRenderer>(*this);
}

void Text::Render() {
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
	else { std::cerr << "ERROR::FONT_MANAGER CAN NOT FIND::CRITICAL" << std::endl; }
}

void Text::SetText(const std::string& new_text) {
    m_text_impl->SetText(new_text);
}

const std::string Text::GetText() const {
    return m_text_impl->GetText();
}

void Text::SetFontSize(const unsigned size) {
    m_text_impl->SetFontSize(size);
}

const unsigned Text::GetFontSize() const {
    return m_text_impl->GetFontSize();
}

bool Text::SetFontName(const std::string& font_tag) {
	if (m_font_manager && m_font_manager->DoesFontExist(font_tag)) {
		m_text_impl->SetFontName(font_tag);
		return true;
	}
	else {
		std::cerr << "ERROR::FONT::NOT::FOUND" << font_tag << std::endl;
		m_text_impl->SetFontName("");
		return false;
	}
}

void Text::SetColor(const glm::vec3 & color) {
    m_text_impl->SetColor(color);
}

}
