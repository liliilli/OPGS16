
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/ObjectTest/object_test_select.h"

#include "../../../Include/Object/ObjectTest/obj_test_mgn.h"

namespace debug::scene {

void ObjectTestSelectScene::Initiate() {
  Instantiate<object::ObjectTestManagerCanvas>("Canvas");
}

} /// ::debug::scene namespace