
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/ObjectTest/node_manager.h"
#include "../../../Include/Script/ObjectTest/node_manager_script.h"

namespace debug::object {

NodeManager::NodeManager() {
  AddComponent<script::NodeManagerScript>(*this);
}

} /// ::debug::object namespace