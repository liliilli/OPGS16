
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Scripts/___3/Private/canvas_script.cc
 * @log
 * 2018-05-13 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "..\..\Include\Script\canvas_script.h"  /// Header file

#include <Element\object.h>
#include <Element\Canvas\canvas.h>
#include <Element\Canvas\text.h>
#include <Component\empty_renderer.h>
#include <Component\sprite_renderer.h>
#include <Manager\sound_manager.h>
#include <Manager\input_manager.h>

namespace {

constexpr const char* k_instruction_name = "Instruction";
constexpr const char* k_instruction_string =
"Debug sound test\n"
"Key 1 : Drumloop BGM ON/OFF\n"
"Key 2 : Jaguar sound effect\n"
"Key 3 : c.ogg sound effect\n"
"Key 4 : Beep sound effect sqns\n";

constexpr const char* k_bgm_sentence_name = "BgmSentence";
constexpr const char* k_info_sentence_name = "InfoSentence";

} /*! unnamed namespace */

namespace opgs16::builtin::sample {

DebugCanvasScript::DebugCanvasScript(element::CObject& bind_object) : 
    CScriptFrame{ bind_object } {
    using EOrigin = IOriginable::Origin;
    using EAlignment = IAlignable::Alignment;

    using CCanvas = element::canvas::CCanvas;
    using CText = element::canvas::CText;

    auto* obj = static_cast<CCanvas*>(&GetObject());

    // Make instruction text object. 
    // This object will display debug test instruction.
    auto instruction = obj->Instantiate<CText>(k_instruction_name, k_instruction_string);
    instruction->SetOrigin(IOriginable::Origin::UP_LEFT);
    instruction->SetFontName("Solomon");
    instruction->SetFontSize(8u);
    instruction->SetWorldPosition(glm::vec3{ 16, -24, 0 });
    instruction->SetColor(glm::vec3{ 1.f, 1.f, 1.f });

    // Bgm sentence object will display BGM ON/OFF status
    // depending on Background music play status on now.
    m_bgm_sentence = obj->Instantiate<CText>(k_bgm_sentence_name, "~BGM OFF~");
    m_bgm_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    m_bgm_sentence->SetAlignment(IAlignable::Alignment::CENTER);
    m_bgm_sentence->SetFontName("Solomon");
    m_bgm_sentence->SetFontSize(8u);
    m_bgm_sentence->SetWorldPosition(glm::vec3{ 0, 48, 0 });

    // Information sentence object will display what is playing now
    // depending on Key 2,3,4 mode.
    m_info_sentence = obj->Instantiate<CText>(k_info_sentence_name, "");
    m_info_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    m_info_sentence->SetAlignment(IAlignable::Alignment::CENTER);
    m_info_sentence->SetFontName("Solomon");
    m_info_sentence->SetFontSize(8u);
    m_info_sentence->SetWorldPosition(glm::vec3{ 0, 32, 0 });

    m_sound = &opgs16::manager::MSoundManager::Instance();

    Start();
}

void DebugCanvasScript::Start() {
    m_sound->CreateSound("Drumloop");
    m_sound->CreateSound("Jaguar");
    m_sound->CreateSound("c_ogg");
    m_sound->CreateSound("Beep1");
    m_sound->CreateSound("Beep2");
    m_sound->CreateSound("Beep3");
}

void DebugCanvasScript::Update() {
    Input();

    switch (m_mode) {
    case 2:
        if (!m_sound->IsPlaying("Jaguar")) {
            m_mode = 0;
            m_info_sentence->SetText("");
        }
        break;
    case 3:
        if (!m_sound->IsPlaying("c_ogg")) {
            m_mode = 0;
            m_info_sentence->SetText("");
        }
        break;
    case 4:
        if (!m_sound->IsPlaying("c_ogg")) {
            m_mode = 0;
            m_info_sentence->SetText("");
        }
        break;
    default: break;
    }
}

void DebugCanvasScript::Input() {
  using ::opgs16::manager::input::IsKeyPressed;
    if (IsKeyPressed("Key1")) {
        if (!m_is_play_bgm) {
            m_sound->PlaySound("Drumloop");
            m_bgm_sentence->SetText("~BGM ON~");
            m_is_play_bgm = true;
        }
        else {
            m_sound->StopSound("Drumloop");
            m_bgm_sentence->SetText("~BGM OFF~");
            m_is_play_bgm = false;
        }
    }
    else if (IsKeyPressed("Key2")) {
        m_sound->PlaySound("Jaguar");
        m_info_sentence->SetText("Now playing Key2 Effect.");
        m_mode = 2;
    }
    else if (IsKeyPressed("Key3")) {
        m_sound->PlaySound("c_ogg");
    }
    else if (IsKeyPressed("Key4")) {
        m_sound->PlaySound("Beep1");
        m_sound->PlaySound("Beep2");
        m_sound->PlaySound("Beep3");
    }
}

} /*! opgs16::builtin::sample */