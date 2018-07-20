
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneMain/ui_title_part.h"

#include "../../../Include/Script/SceneMain/script_component.h"
#include "../../../Include/Script/SceneMain/script_select.h"
#include "../../../Include/Script/SceneMain/script_title.h"

namespace magiccup {

UiTitlePart::UiTitlePart() {
  SetScaleFactor({128.f, 112.f, 0.f});
  this->AddComponent<ScriptTitleSelect>(*this);
  this->AddComponent<ScriptTitleDisplay>(*this);
  this->AddComponent<ScriptTitleComponent>(*this);

  this->SetObjectActive(false);
}

} /// ::magiccup namespace