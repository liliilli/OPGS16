#ifndef DEBUG_PROJECT_SCENE_OBJECTTEST_OBJECT_TEST_SELECT_H
#define DEBUG_PROJECT_SCENE_OBJECTTEST_OBJECT_TEST_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/scene.h>

namespace debug::scene {

class ObjectTestSelectScene final : public opgs16::element::CScene {
public:
  void Initiate() override final;
};

} /// ::debug::scene namespace

#endif /// DEBUG_PROJECT_SCENE_OBJECTTEST_OBJECT_TEST_SELECT_H