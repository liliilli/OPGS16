
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/InputTest/input_select.h"
#include "../../../Include/Script/InputTest/select_lobby.h"

namespace debug::object {

InputSelectObject::InputSelectObject() {
  AddComponent<script::InputTestLobbyScript>(*this);
}

} /// ::debug::object namespace