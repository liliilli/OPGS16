#ifndef OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H
#define OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H

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
 * @file GlobalObjects/Canvas/text.h
 * @brief Text object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace Draw() with Render()
 * 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
 * 2018-04-17 Move ::canvas::CText to ::opgs16::element::canvas::CText.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <memory>       /*! std::unique_ptr */
#include <glm/glm.hpp>  /*! using glm::vec3<float, 0> = glm::tvec3<float, 0> */
#include "../../GlobalObjects/Interface/i_alignable.h"  /*! IAlignable */
#include "../../Headers/Fwd/objectfwd.h"    /*! MFontManager
                                              * Canvas::CTextImpl
                                              * Canvas::TextImplDeleter
                                              * glm::vec3 */
#include "../../System/Element/Public/ui_object.h"      /*! ::opgs16::element::UiObject */

namespace opgs16 {
namespace element {
namespace canvas {
/*!
 * @class CText
 * @brief This class display text on position aligned with FontManager.
 * @log
 * 2018-04-17 Move ::canvas::CText to ::opgs16::element::canvas::CText.
 */
class CText : public opgs16::element::UiObject, public IAlignable {
public:
    /**
     * @brief Text component (stand-alone) constructor.
     * In initialization time of this class, initial text and position and color set.
     * position parameter was based on screen or parent's size where component is in hierarchy
     * structrue of parent.
     */
	CText(const std::string& initial_txt,
          const glm::vec3& position = glm::vec3{ 0.f, 0.f, 0.f },
          const glm::vec3& color = glm::vec3{ 1.f, 1.f, 1.f });

	virtual ~CText() = default;

	void Render() override final;

    /**
     * @brief Set text string to display.
     * @param[in] new_text New text string.
     */
	void SetText(const std::string& new_text);

    /**
     * @brief Get text string to display.
     * @return text string which this instance has.
     */
    const std::string GetText() const;

	/**
	 * @brief Set font size of string.
     * This method has side-effect for accessing FontManager.
	 * @param[in] size;
	 */
	void SetFontSize(const unsigned size);

    /**
     * @brief Get font size which this instance has.
     * @return unsigned font size value.
     */
    const unsigned GetFontSize() const;

	/**
	 * @brief Set this object font to render with.
	 * If FontManager doesn't find font, return false to inform method was failed.
	 * @param[in] font_tag Font name tag.
	 * @return Success flag.
	 */
	bool SetFontName(const std::string& font_tag);

    /**
     * @brief Set string color, single color with composited of {r, g, b}.
     * @param[in] color New color value with consists of {r, g, b} glm::vec3 vector.
     * each value must be in range of [0, 1], otherwise clamped to 0 or 1.
     */
    void SetColor(const glm::vec3& color);

    /**
     * @brief Get reference of FontManager which this class grasps.
     * Returning reference is attached to cv-qualifier (const), Caller can use const method
     * does not alter bit-consistancy of FontManager.
     * @return Const reference of FontManager which this class grasps (must not be a nullptr.)
     */
    const opgs16::manager::MFontManager& GetFontManager() const {
        return *m_font_manager;
    }

private:
    /** private implementation instance */
    std::unique_ptr<opgs16::element::canvas::_internal::CTextImpl,
    opgs16::element::canvas::_internal::TextImplDeleter> m_text_impl{ nullptr };
	opgs16::manager::MFontManager* const m_font_manager{ nullptr };
};
} /*! opgs16::element::canvas */
} /*! opgs16::element */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_ELEMENT_CANVAS_PUBLIC_TEXT_H */
