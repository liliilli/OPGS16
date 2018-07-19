
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Object/SceneGameLogo/logo_effect.h"

#include "../../../Include/Script/SceneGameLogo/script_logo_effect.h"

namespace magiccup {

UiLogoObject::UiLogoObject() {
  AddComponent<ScriptLogoEffect>(*this);
}

void UiLogoObject::Render() { }

} /// ::magiccup namespace