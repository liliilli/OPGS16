
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/Test/random.h"

#include "../../../Include/Object/RandomTest/manager.h"

namespace debug::scene {

void RandomTestScene::Initiate() {
  Instantiate<object::RandomManager>("Canvas");
}

} /// ::debug::scene namespace