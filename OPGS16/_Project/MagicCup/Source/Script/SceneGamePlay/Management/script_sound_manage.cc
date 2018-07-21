
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/Management/script_sound_manage.h"

#include <Manager/sound_manager.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

void ScriptSoundManage::Initiate() {
  using opgs16::manager::sound::GenerateSound;

  GenerateSound(keyword::eff_cursormove);
  GenerateSound(keyword::eff_decision);
  GenerateSound(keyword::eff_next);
  GenerateSound("EffWarning");
  GenerateSound("EffNext");
  GenerateSound("BgmSchool");
}

void ScriptSoundManage::Destroy() {
  using opgs16::manager::sound::DestroySound;

  DestroySound(keyword::eff_cursormove);
  DestroySound(keyword::eff_decision);
  DestroySound(keyword::eff_next);
  DestroySound("EffWarning");
  DestroySound("EffNext");
  DestroySound("BgmSchool");
}

} /// ::magiccup namespace