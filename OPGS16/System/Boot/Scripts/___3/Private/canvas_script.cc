
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

#include "../Public/canvas_script.h"               /*! Header file */

#include "../../../../Element/Public/object.h"
#include "../../../../Element/Canvas/Public/canvas.h"       /*! ::opgs16::element::canvas::CCanvas */
#include "../../../../Element/Canvas/Public/text.h"         /*! ::opgs16::element::canvas::CText */
#include "../../../../Components/Public/empty_renderer.h"   /*!*/
#include "../../../../Components/Public/sprite_renderer.h"  /*! opgs16::component::CSprite2DRenderer */

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
    auto bgm_sentence = obj->Instantiate<CText>(k_bgm_sentence_name, "TESTTEST");
    bgm_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    bgm_sentence->SetAlignment(IAlignable::Alignment::CENTER);
    bgm_sentence->SetFontName("Solomon");
    bgm_sentence->SetFontSize(8u);
    bgm_sentence->SetWorldPosition(glm::vec3{ 0, 48, 0 });

    // Information sentence object will display what is playing now
    // depending on Key 2,3,4 mode.
    auto info_sentence = obj->Instantiate<CText>(k_info_sentence_name, "TESTTEST");
    info_sentence->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    info_sentence->SetAlignment(IAlignable::Alignment::CENTER);
    info_sentence->SetFontName("Solomon");
    info_sentence->SetFontSize(8u);
    info_sentence->SetWorldPosition(glm::vec3{ 0, 32, 0 });
}

void DebugCanvasScript::Start() {
}

void DebugCanvasScript::Update() {
}

} /*! opgs16::builtin::sample */