
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Scene/Test/sound.h"
#include "../../../Include/Object/SoundTest/sound_select.h"

namespace debug::scene {

void SoundTestScnee::Initiate() {
  Instantiate<object::SoundSelect>("Canvas");
}

} /// ::debug::scene namespace