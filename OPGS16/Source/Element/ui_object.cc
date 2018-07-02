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
 * @file System/Element/Private/ui_object.cc
 * @log
 * 2018-03-11 Add boilerplate comment, replace it within ::opgs16::element.
 */

#include <Element\ui_object.h>  /// Header file

#include <array>                /// std::array

#include <glm\glm.hpp>

namespace opgs16 {
namespace element {

void UiObject::LocalUpdate() {
    for (auto& child : m_children) {
        if (child.second && child.second->GetActive()) {
            auto child_temp = static_cast<UiObject*>(child.second.get());
            child_temp->SetUiParentPosition(screen_x, screen_y, screen_width, screen_height);
        }
    }
}

void UiObject::SetUiParentPosition(const float parent_x, const float parent_y,
                                   const float parent_width, const float parent_height) {
	auto origin	= static_cast<int>(GetOrigin()) - 1;
	unsigned x = origin % 3;
	unsigned y = origin / 3;

	GLint source_x = static_cast<GLint>(parent_x + (parent_width / 2) * x);
	GLint source_y = static_cast<GLint>(parent_y + (parent_height / 2) * y);

  SetParentPosition( DVector3{
      static_cast<float>(source_x),
      static_cast<float>(source_y),
      0.f }
  );
	for (auto& child : GetChildList()) {
		/** TODO :: NEED PERFORMANCE CHECK */
		auto child_temp = static_cast<UiObject*>(child.second.get());
		child_temp->SetUiParentPosition(screen_x, screen_y, screen_width, screen_height);
	}
}

void UiObject::UpdateScreenXYWH(const std::array<GLint, 4> xywh) {
	screen_x		= static_cast<float>(xywh[0]);
	screen_y		= static_cast<float>(xywh[1]);
	screen_width	= static_cast<float>(xywh[2]);
	screen_height	= static_cast<float>(xywh[3]);
}

} /*! opgs16::element */
} /*! opgs16 */
