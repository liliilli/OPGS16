#ifndef DEBUG_SCENE_DEBUG_TEXT_H
#define DEBUG_SCENE_DEBUG_TEXT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Include/Scene/debug_text.h
///
/// @brief
/// Text debug scene.
///
/// @log
/// 2018-05-13 Create file.
///

/// ::opgs16::element::CScene
#include <Element/scene.h>

namespace debug::scene {

class DebugText final : public opgs16::element::CScene {
  void Initiate() override final;
};

}

#endif /// DEBUG_SCENE_DEBUG_TEXT_H