
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGameLogo/logo_char.h"

#include "../../../Include/Script/SceneGameLogo/script_char_jumping.h"

namespace magiccup {

UiLogoChar::UiLogoChar() {
  AddComponent<ScriptCharJumping>(*this);
}

} /// ::magiccup namespace