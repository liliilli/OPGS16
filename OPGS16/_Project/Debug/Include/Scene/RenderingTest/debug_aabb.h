#ifndef DEBUG_SCENE_DEBUG_AABB_H
#define DEBUG_SCENE_DEBUG_AABB_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Include/Scene/debug_aabb.h
/// @brief
///
/// @author Jongmin Yun
/// @log
/// 2018-06-01 Create file.
///

/// ::opgs16::element::CScene
#include <Element/scene.h>

namespace debug::scene {

class DebugAABB final : public opgs16::element::CScene {
  void Initiate() override final;
};

} /// ::debug::scene

#endif /// DEBUG_SCENE_DEBUG_AABB_H