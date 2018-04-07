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
 * @file System/Boot/Scripts/__b_scr.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-07 Activate file and add comments.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__b_scr.h"  /*! Header file */

#include <sstream>
#include <glm/glm.hpp>
#include "../../../../GlobalObjects/Canvas/text.h"      /*! Canvas::Text */
#include "../../../../GlobalObjects/Canvas/image.h"     /*! Canvas::Image */
#include "../../../../System/Manager/Public/scene_manager.h"  /*! SceneManager */
#include "../../../../System/Manager/Public/timer_manager.h"  /*! TimerManager */
#include "../../../../Headers/import_logger.h" /*! import logger in debug mode */

#include "../../Scene/Public/__sample.h"
#include "../../../Core/Public/core_setting.h"
#include "../../../manifest.h"

__B_SCR::__B_SCR(opgs16::element::CObject& obj) : CScriptFrame{ obj } {
    Initiate();
    Start();
}

void __B_SCR::Start() {
    PlaySoundEffect();
    M_SET_TIMER(m_timer, 32, true, this, &__B_SCR::MoveLogo);
}

void __B_SCR::OnTriggerTimerBreak() { }

void __B_SCR::PlaySoundEffect() {
    //auto& sound_manager = SoundManager::GetInstance();
    //sound_manager.CreateSound("__system49");
    //sound_manager.PlaySound("__system49");
}

void __B_SCR::MoveLogo() {
    static float elapsed{ 0.f };
    auto logo = GetObject().GetChild("Logo");
    if (!logo) return;

    auto pos = logo->GetWorldPosition();
    constexpr float total = 2.0f;
    constexpr float mult = 160.f / total;
    elapsed += 0.032f;
    pos.y = powf(elapsed - total, 2) * mult;
    if (abs(pos.y) < 0.1f) {
        PUSH_LOG_INFO(L"Logo soft-randing");
        pos.y = 0;
        opgs16::manager::MTimerManager::Instance().DetachTimer(m_timer);
        M_SET_TIMER(m_timer_2, 500, false, this, &__B_SCR::CreateTextObject);
    }

    logo->SetWorldPosition(pos);
}

void __B_SCR::CreateTextObject() {
    std::stringstream string_stream;
    string_stream << 'v' << _OPGS16_VERSION_MAJOR << '.' << _OPGS16_VERSION_MINOR << '.' << _OPGS16_VERSION_FIXED;

    std::string text_string; string_stream >> text_string;
    text_string.append("\n\n"
                       "VIDEO RAM:640KiBytes\n"
                       "MAIN RAN:1024KiByes\n\n");
    text_string.append("DOES NOT FIND ANY ROM.\n"
                       "BOOT SAMPLE GAME...");
        auto text = std::make_unique<canvas::Text>(text_string);
    {
        text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
        text->SetWorldPosition({ 0, -48, 0 });
        text->SetAlignment(IAlignable::Alignment::CENTER);
        text->SetFontName("Solomon");
        text->SetFontSize(8u);
        text->SetColor(glm::vec3{ 1, 1, 1 });
    }
    GetObject().Instantiate<canvas::Text>("Statement", text);
    M_SET_TIMER(m_timer, 2'000, false, this, &__B_SCR::OnTriggerNextScene);
}

void __B_SCR::OnTriggerNextScene() {
    PUSH_LOG_INFO("Booting scene to sample game scene...");
#if !defined(_CUSTOM_PROJECT)
    M_REPLACE_SCENE(opgs16::builtin::sample::SampleGame);
#else
    M_POP_SCENE()
#endif
}