
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SoundTest/sound_select.h"
#include "../../../Include/Script/SoundTest/root.h"

namespace debug::object {

SoundSelect::SoundSelect() {
  AddComponent<script::SoundTestRoot>(*this);
}

} /// ::debug::object namespace