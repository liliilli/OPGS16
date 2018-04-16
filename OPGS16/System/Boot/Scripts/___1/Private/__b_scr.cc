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
 * @file System/Boot/Scripts/Private/__b_scr.cc
 * @author Jongmin Yun
 * @log
 * 2018-04-07 Activate file and add comments.
 * 2018-04-14 Move definitions into ::opgs16::builtin::sample namespace.
 *!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__b_scr.h"  /*! Header file */

#include <chrono>
#include <random>
#include <sstream>

#include <glm/glm.hpp>

#include "../../../../Components/Public/sprite_renderer.h" /*! opgs16::component::CSprite2DRenderer */
#include "../../../../Manager/Public/scene_manager.h"  /*! SceneManager */
#include "../../../../Manager/Public/timer_manager.h"  /*! TimerManager */
#include "../../../../Shader/shader_wrapper.h"
#include "../../../../../GlobalObjects/Canvas/canvas.h"    /*! Canvas::CCanvas */
#include "../../../../../GlobalObjects/Canvas/text.h"      /*! Canvas::CText */
#include "../../../../../GlobalObjects/Canvas/image.h"     /*! Canvas::CImage */
#include "../../../../../Headers/import_logger.h" /*! import logger in debug mode */
#include "../../../../Core/Public/core_setting.h"
#include "../../../../../manifest.h"

#include "../../../Scene/Public/__sample.h"

namespace {
constexpr const char*   shader_sliced{ "__b_sliced" };
constexpr const int     k_sliced_number{ 32 };
constexpr const float   k_sliced_y_initial_scale{ 128.f };
constexpr const float   k_sliced_y_final_scale{ 9.5f };

int y_positions[k_sliced_number << 1]{ 0, };
int y_initial[k_sliced_number << 1]{ 0, };
} /*! unnamed namespace */

namespace opgs16 {
namespace builtin {
namespace sample {

__B_SCR::__B_SCR(opgs16::element::CObject& obj) : CScriptFrame{ obj } {
    Initiate();
    Start();
}

void __B_SCR::Start() {
    PlaySoundEffect();
    M_SET_TIMER(m_timer_2, 1'000, false, this, &__B_SCR::SetLogoImage);
}

void __B_SCR::PlaySoundEffect() {
    //auto& sound_manager = SoundManager::GetInstance();
    //sound_manager.CreateSound("__system49");
    //sound_manager.PlaySound("__system49");
}

void __B_SCR::SetLogoImage() {
    std::default_random_engine rng;
    rng.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    logo = GetObject().Instantiate<element::canvas::CImage>("Logo", "System",
                                                            static_cast<element::canvas::CCanvas*>(&GetObject()));
    component::CSprite2DRenderer* renderer = logo->GetComponent<opgs16::component::CSprite2DRenderer>();
    renderer->SetTextureIndex(4);
    logo->SetImageSize(178.f, 19.f);

    if ((rng() % 1'000) < 500) { // Normal
        logo->SetWorldPosition({ 0, 160, 0 });
        M_SET_TIMER(m_timer, 32, true, this, &__B_SCR::MoveLogo1);
    }
    else { // Sliced
        for (int i = 0; i < (k_sliced_number << 1); ++i) {
            y_positions[i] = (rng() % 400 + 200) + 112;
            y_initial[i] = y_positions[i];
        }

        renderer->SetShader(shader_sliced);
        renderer->SetInstanceCount(k_sliced_number << 1);
        auto& wrapper = renderer->Wrapper();
        wrapper.SetUniformValueInt("uNumber", k_sliced_number << 1);
        wrapper.SetUniformValueIntPtr("uPos", y_positions, k_sliced_number << 1);
        wrapper.SetUniformValue<float>("uYScale", k_sliced_y_initial_scale);
        M_SET_TIMER(m_timer, 16, true, this, &__B_SCR::MoveLogoSliced);
    }
}

void __B_SCR::MoveLogo1() {
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

void __B_SCR::MoveLogoSliced() {
    using opgs16::component::CSprite2DRenderer;

    static float elapsed{ 0.f };
    constexpr float time{ 0.75f };
    constexpr float time_pow{ 0.5625f };
    constexpr float offset_scale{ k_sliced_y_initial_scale / time_pow };

    elapsed += 0.016f;
    const auto sigma = powf(time - elapsed, 2);
    for (int i = 0; i < (k_sliced_number << 1); ++i)
        y_positions[i] = static_cast<int>(sigma * (y_initial[i] / time_pow));

    auto* renderer = logo->GetComponent<CSprite2DRenderer>();
    auto& wrapper  = renderer->Wrapper();

    wrapper.SetUniformValue<float>("uYScale", sigma / offset_scale + k_sliced_y_final_scale);

    //wrapper.SetUniformValue<float>(
    //    "uYScale", k_sliced_y_initial_scale * (1 - powf(elapsed, 2) / time_pow) + k_sliced_y_final_scale);

    if (elapsed >= time) {
        PUSH_LOG_INFO(L"Logo soft-randing");
        renderer->SetShader("gQuad");
        renderer->SetInstanceCount(1);
        opgs16::manager::MTimerManager::Instance().DetachTimer(m_timer);
        M_SET_TIMER(m_timer_2, 500, false, this, &__B_SCR::CreateTextObject);
    }
}

void __B_SCR::CreateTextObject() {
    std::stringstream string_stream;
    string_stream << 'v' << _OPGS16_VERSION_MAJOR << '.' << _OPGS16_VERSION_MINOR << '.' << _OPGS16_VERSION_FIXED;

    std::string text_string; string_stream >> text_string;
    text_string.append("\n\n"
                       "VIDEO RAM:640KiBytes\n"
                       "MAIN RAN:1024KiByes\n\n");
    text_string.append("DID NOT FIND ANY ROM.\n"
                       "BOOT SAMPLE GAME...");
    auto text = std::make_unique<element::canvas::CText>(text_string);
    {
        text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
        text->SetWorldPosition({ 0, -48, 0 });
        text->SetAlignment(IAlignable::Alignment::CENTER);
        text->SetFontName("Solomon");
        text->SetFontSize(8u);
        text->SetColor(glm::vec3{ 1, 1, 1 });
    }
    GetObject().Instantiate<element::canvas::CText>("Statement", text);
    M_SET_TIMER(m_timer, 2'000, false, this, &__B_SCR::OnTriggerNextScene);
}

void __B_SCR::OnTriggerNextScene() {
    PUSH_LOG_INFO("Booting scene to sample game scene...");
#if !defined(_CUSTOM_PROJECT)
    M_REPLACE_SCENE(opgs16::builtin::sample::SampleGame);
#else
    M_POP_SCENE();
#endif
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16::sample */
