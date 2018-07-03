
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneSelect/select_manager.h"

#include "../../../Include/Script/SceneSelect/management.h"

namespace debug::object {

SelectManager::SelectManager() : CCanvas() {
  auto script = AddComponent<script::Management>(*this);
}

} /// ::debug::object namespace