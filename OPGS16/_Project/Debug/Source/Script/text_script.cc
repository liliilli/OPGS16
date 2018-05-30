
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Script/text_script.cc
///

/// Header file
#include "../../Include/Script/text_script.h"

#include <Element/Canvas/text.h>
#include "Element/Canvas/canvas.h"

namespace {
  constexpr const char* test_text1 =
    u8"0123456789`~!@#$%^&*()\\|;:'\"{[]}\n"
    u8",<.>/? ABCDEFGHIJKLMNOPQRSTUVWX\n"
    u8"YZ abcdefghijklmnopqrstuvwxyz";

  constexpr const char* test_text2 =
    u8"한글 Test.\n"
    u8"아기 : 부, 분하다...... 뜻을\n"
    u8"다 이루지 못하고 나는 죽어야\n"
    u8"만 하는 것인가......";

  constexpr const char* test_text3 =
    u8"くお～!!ぶつかる～!!\n"
    u8"ここでアクセル全開、\n"
    u8"インド人を右に！";
}

namespace debug::script {

void TextScript::Initiate() {}

void TextScript::Start() {
  if (!is_created) {
    auto& obj = GetBindObject();
    auto text1 = obj.Instantiate<opgs16::element::canvas::CText>(
        "Text1", test_text1);
    text1->SetOrigin(IOriginable::Origin::UP_LEFT);
    text1->SetFontName("Solomon");
    text1->SetFontSize(8u);
    text1->SetWorldPosition({ 0, -16, 0 });
    text1->SetColor(glm::vec3{1.f, 1.f, 0.f });

    auto text2 = obj.Instantiate<opgs16::element::canvas::CText>(
        "Text2", test_text2);
    text2->SetOrigin(IOriginable::Origin::CENTER_LEFT);
    text2->SetFontName("Sam3");
    text2->SetFontSize(16u);
    text2->SetWorldPosition({ 0, 0, 0 });
    text2->SetColor(glm::vec3{1.f, 0.f, 1.f});

    auto text3 = obj.Instantiate<opgs16::element::canvas::CText>(
        "Text3", test_text3);
    text3->SetOrigin(IOriginable::Origin::DOWN_LEFT);
    text3->SetFontName("Pixel");
    text3->SetFontSize(16u);
    text3->SetWorldPosition({ 0, 48, 0 });
    text3->SetColor(glm::vec3{0.f, 1.f, 1.f});

    is_created = true;
  }
}

void TextScript::Update() {

}

void TextScript::Destroy() {

}

} /// ::debug::script namespace.