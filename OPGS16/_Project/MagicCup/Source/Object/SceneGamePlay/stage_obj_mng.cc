
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/stage_obj_mng.h"

#include "../../../Include/Script/SceneGamePlay/script_obj_mng.h"

namespace magiccup {

StageObjectManagement::StageObjectManagement() {
  AddComponent<ScriptObjectManagement>(*this);
}

} /// ::magiccup namespace