
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Scoreboard/ui_scoreboard.h"

#include "../../../Include/Script/Scoreboard/script_scoreboard.h"

namespace magiccup {

UiScoreboard::UiScoreboard() {
  this->AddComponent<ScriptScoreboard>(*this);
}

} /// ::magiccup namespace