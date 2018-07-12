
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/ObjectTest/obj_test_mgn.h"
#include "../../../Include/Script/ObjectTest/obj_test_mgn_script.h"

namespace debug::object {

ObjectTestManagerCanvas::ObjectTestManagerCanvas() {
  AddComponent<script::ObjectTestManagerScript>(*this);
}

} /// ::debug::object namespace