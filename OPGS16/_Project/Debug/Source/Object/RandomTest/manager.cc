
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/RandomTest/manager.h"
#include "../../../Include/Script/RandomTest/manager.h"

namespace debug::object {

RandomManager::RandomManager() {
  AddComponent<script::RandomTestManager>(*this);
}

} /// ::debug::object namespace