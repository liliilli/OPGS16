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
 * @file System/Boot/Object/__2/Public/__s_canvas.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-14 Create file.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_canvas.h"           /*! Header file */
#include "../../../../Components/Public/empty_renderer.h"
#include "../../../../Element/Canvas/Public/text.h" /*! ::opgs16::element::canvas::CText */
#include "../../../../../GlobalObjects/Interface/i_alignable.h"

constexpr const char* k_text_name = "AngleText";

namespace opgs16 {
namespace builtin {
namespace sample {

__S_CANVAS::__S_CANVAS() {
    auto text = Instantiate<element::canvas::CText>(k_text_name, "360'00''");
    text->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    text->SetWorldPosition({ 0, 32, 0 });
    text->SetAlignment(IAlignable::Alignment::CENTER);
    text->SetFontName("BIOS");
    text->SetFontSize(16u);
    text->SetColor(glm::vec3{ .5f, .5f, .5f });

    if (auto renderer = text->GetComponent<component::CEmptyRenderer>(); renderer)
        renderer->SetRenderLayer(0);
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! ogps16 */
