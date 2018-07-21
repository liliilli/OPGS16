
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Gameover/ui_char.h"
#include "../../../../Include/Script/SceneGamePlay/GameOver/script_char.h"

namespace magiccup {

UiCharRouletteObject::UiCharRouletteObject() : CText{""} {
  this->SetFontName("opSystem");
  this->SetFontSize(16);
  this->SetAlignment(Alignment::CENTER);
  this->SetRenderingLayer("Foremost");
  this->AddComponent<ScriptCharRoulette>(*this);
}

} /// ::magiccup namespace