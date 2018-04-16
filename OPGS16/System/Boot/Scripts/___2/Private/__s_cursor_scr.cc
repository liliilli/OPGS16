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

#include "../Public/__s_cursor_scr.h"
#include "../../../../Manager/Public/input_manager.h"
#include "../../../../Manager/Public/scene_manager.h"
#include "../../../../Element/Public/object.h"

#include "../../../../../GlobalObjects/Canvas/text.h"

constexpr float k_angle_offset = 3.1725f;

namespace opgs16 {
namespace builtin {
namespace sample {

__S_CURSOR_SCR::__S_CURSOR_SCR(opgs16::element::CObject& bind_object) :
    CScriptFrame{ bind_object }, m_input_manager{ GET_INPUT_MANAGER } {
    Initiate();
    Start();
}

void __S_CURSOR_SCR::Start() {
    GetObject().SetRotationLocalFactor({ 0, 0, 1 });
}

void __S_CURSOR_SCR::Update() {
    if (const auto h_val = m_input_manager.GetKeyValue("Horizontal"); h_val) {
        auto& object = GetObject();

        float angle_value = object.GetRotationLocalAngle();
        angle_value += k_angle_offset * h_val;
        object.SetRotationLocalAngle(angle_value);

        auto canvas = manager::MSceneManager::Instance().PresentScene()->GetObject("Canvas").get();
        auto text = static_cast<canvas::Text*>(canvas->GetChild("AngleText"));

        char text_string[10]{};
        sprintf(text_string, "%.1f", object.GetRotationLocalAngle());
        strcat(text_string, "'");
        text->SetText(text_string);
    }
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */