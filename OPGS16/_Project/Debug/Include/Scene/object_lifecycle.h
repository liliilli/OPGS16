#ifndef PROJECT_DEBUG_SCENE_OBJECT_LIFECYCLE_H
#define PROJECT_DEBUG_SCENE_OBJECT_LIFECYCLE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Scene/object_lifecycle.h
///

/// ::opgs16::element::CScene
#include <Element/scene.h>

namespace debug::scene {

class ObjectLifeCycle final : public opgs16::element::CScene {
  void Initiate() override final;
};

} /// ::debug::scene


#endif /// PROJECT_DEBUG_SCENE_OBJECT_LIFECYCLE_H