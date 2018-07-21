
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Gameover/ui_nameentry.h"
#include "../../../../Include/Script/SceneGamePlay/GameOver/script_nameentry.h"

namespace magiccup {

UiNameEntryObject::UiNameEntryObject() {
  this->AddComponent<ScriptNameEntry>(*this);
}

} /// ::magiccup namespace