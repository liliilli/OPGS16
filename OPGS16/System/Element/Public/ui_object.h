#ifndef OPGS16_SYSTEM_OBJECT_UI_OBJECT_H
#define OPGS16_SYSTEM_OBJECT_UI_OBJECT_H

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
 * @file System\Object\ui_object.h
 * @brief Base clase for m_object_list inherit UiObject. (UI component)
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Remove Draw() override and Draw(ShaderNew) deleted method.
 * 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
 */

#include <GL/glew.h>
#include "object.h"                                     /*! Object */
#include "../../GlobalObjects/Interface/i_originable.h" /*! IOriginable */

namespace opgs16 {
namespace element {

/*!
 * @class UiObject
 * @brief UiObject class would be used to implement Ui component, such as Canvas, Image, etc.
 * First, UiObject has slightly different Update mechanism, to display components hierarcherly.
 *
 * Second, UiObject has only a child inherits UiObject class.
 * Otherwise, undefined behavior will occur (I swear)
 */
class UiObject : public Object, public IOriginable {
public:
	virtual ~UiObject() = default;

    /*! Uiobject updates children edge position */
    virtual void LocalUpdate() override;

protected:
	/**
	 * @brief Updates screen_x, screen_y, width, height coordinate value in Screen space.
	 * This methods updates caller's screen_x, y, width, height itself.
	 * @param[in] parent_xywh Caller's screen_x,y,width,height position array.
	 */
	 void UpdateScreenXYWH(const std::array<GLint, 4> parent_xywh);

private:
	float screen_x{}, screen_y{};
	float screen_width{}, screen_height{};

private:
	/**
	 * @brief Updates callee(child)'s final position to be rendered.
	 *
	 * This method gets parent(Caller)'s x, y, width, height value of Screen space,
	 * and calculate rendering position referring to IOriginable::Origin value of child.
	 * Detail mechanism is below.
	 * \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate
	 * -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without
	 * /(1)|(2)\(3) applying child's local position.
	 * Afterward, this method sets final rendering position of child and apply changed value to
	 * children of child calling SetUiParentPosition() of children of child.
	 */
	 void SetUiParentPosition(const float parent_x, const float parent_y,
                              const float parent_width, const float parent_height);
};

} /*! opgs16::element */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_OBJECT_UI_OBJECT_H */
