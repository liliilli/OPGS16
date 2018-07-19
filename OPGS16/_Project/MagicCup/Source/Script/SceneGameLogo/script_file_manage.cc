
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGameLogo/script_file_manage.h"

#include "../../../Include/Global/data_singleton.h"

namespace magiccup {

void ScriptFileManage::Initiate() {
  if (data::IsSaveFileExist()) {
    data::LoadSaveFile();
  }
  else {
    data::SaveSaveFile();
  }
}

} /// ::magiccup namespace