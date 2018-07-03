#ifndef PROJECT_DEBUG_SCENE_SELECT_SCENE_H
#define PROJECT_DEBUG_SCENE_SELECT_SCENE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// ::opgs16::element::CScene
#include <Element/scene.h>

namespace debug::scene {

class SelectScene final : public opgs16::element::CScene {
  void Initiate() override final;
};

} /// ::debug::scene

#endif /// PROJECT_DEBUG_SCENE_SELECT_SCENE_H