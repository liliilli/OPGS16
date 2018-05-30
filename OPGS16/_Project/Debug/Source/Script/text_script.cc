
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

namespace debug::script {

void TextScript::Initiate() {
  auto& obj = GetBindObject();
  auto text1 = obj.Instantiate<opgs16::element::canvas::CText>(
      "Text1", "Ascii, Hello world! ():=).");
  text1->SetOrigin(IOriginable::Origin::UP_LEFT);
  text1->SetFontName("Solomon");
  text1->SetFontSize(8u);
  text1->SetWorldPosition({ 16, -24, 0 });
  text1->SetColor(glm::vec3{1.f, 1.f, 0.f });
}

void TextScript::Start() {}

void TextScript::Update() {

}

void TextScript::Destroy() {

}

} /// ::debug::script namespace.