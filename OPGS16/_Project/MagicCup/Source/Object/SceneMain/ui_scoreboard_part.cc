
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneMain/ui_scoreboard_part.h"

#include "../../../Include/Script/Scoreboard/script_scoreboard.h"

namespace magiccup {

UiScoreboardPart::UiScoreboardPart() {
  SetScaleFactor({128.f, 112.f, 0.f});
  this->AddComponent<ScriptScoreboard>(*this);

  this->SetObjectActive(false);
}

} /// ::magiccup namespace