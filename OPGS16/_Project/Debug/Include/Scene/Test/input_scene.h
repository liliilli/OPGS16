#ifndef DEBUG_PROJECT_SCENE_TEST_INPUT_SCENE_H
#define DEBUG_PROJECT_SCENE_TEST_INPUT_SCENE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/scene.h>

namespace debug::scene {

class InputTestScene final : public opgs16::element::CScene {
  void Initiate() override final;
};

} /// ::debug::scene namespace

#endif /// DEBUG_PROJECT_SCENE_TEST_INPUT_SCENE_H