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
 * @file System/Scripts/___2/Private/__s_script_game_canvas.cc
 * @brief Implementation file of ../Public/__s_script_game_canvas.h.
 * @log
 * 2018-04-15 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_script_game_canvas.h"   /*! Header file */

#include "../../../../Element/Public/object.h"
#include "../../../../../GlobalObjects/Canvas/text.h"       /*! ::canvas::Text */
#include "../../../../../GlobalObjects/Canvas/image.h"      /*! ::canvas::Image */
#include "../../../../Components/Public/empty_renderer.h"   /*!*/
#include "../../../../Components/Public/sprite_renderer.h"  /*! opgs16::component::CSprite2DRenderer */
#include "../../../Object/___2/Public/__s_game_canvas.h"

constexpr const char* k_score_name          = "Score";
constexpr const char* k_score_value_name    = "ScoreValue";
constexpr const char* k_life_txt_name       = "LifeText";
constexpr const char* k_life_img_name       = "LifeImage";
constexpr const char* k_title_name          = "Title";
constexpr const char* k_title_string        =
"SPACE CHASER\n"
"\n"
"-- SCORE --\n"
"01234567890\n"
"01234567890\n"
"01234567890\n"
"01234567890\n";

namespace opgs16 {
namespace builtin {
namespace sample {

__S_SCRIPT_GAME_CANVAS::__S_SCRIPT_GAME_CANVAS(opgs16::element::CObject& bind_object) : CScriptFrame{ bind_object } {
    using EOrigin = IOriginable::Origin;
    using EAlignment = IAlignable::Alignment;
    const char* text_name_array[4]{ k_score_name, k_life_txt_name, k_title_string, k_title_name };
    const char* text_init_array[4]{ "SCORE", "00000", "LIFE", k_title_string };
    const EOrigin origin_array[4] { EOrigin::UP_LEFT, EOrigin::UP_LEFT, EOrigin::UP_RIGHT, EOrigin::UP_CENTER };
    const EAlignment alignments[4]{ EAlignment::LEFT, EAlignment::LEFT, EAlignment::RIGHT, EAlignment::CENTER };
    const glm::vec3 position_array[4]{ {16, -24, 0}, {16, -32, 0}, {-16, -24, 0}, {0, -32, 0} };
    const glm::vec3 color_array[4]{ {1.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, {1.f, 0.f, 0.f}, {1.f, 1.f, 0.f} };

    auto* obj = static_cast<__S_GAME_CANVAS*>(&GetObject());
    for (auto i = 0u; i < 4u; ++i) {
        auto text_obj = obj->Instantiate<canvas::Text>(text_name_array[i], text_init_array[i]);
        text_obj->SetOrigin(origin_array[i]);
        text_obj->SetAlignment(alignments[i]);
        text_obj->SetFontName("Solomon");
        text_obj->SetFontSize(8u);
        text_obj->SetWorldPosition(position_array[i]);
        text_obj->SetColor(color_array[i]);
        if (auto renderer = text_obj->GetComponent<component::CEmptyRenderer>(); renderer) renderer->SetRenderLayer(0);
        if (i == 1)
            m_score_obj = text_obj;
    }

    for (auto i = 0u; i < 3; ++i) {
        canvas::Image* life = obj->Instantiate<canvas::Image>(k_life_img_name, "System", obj);
        life->SetImageSize(12.f, 12.f);
        life->SetWorldPosition({ 80 + (14 * i), 84, 0 });
        auto renderer = life->GetComponent<component::CSprite2DRenderer>();
        renderer->SetTextureIndex(14);

        m_life_objects.emplace_back(life);
    }
}

void __S_SCRIPT_GAME_CANVAS::Start() {
}

void __S_SCRIPT_GAME_CANVAS::Update() {
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */