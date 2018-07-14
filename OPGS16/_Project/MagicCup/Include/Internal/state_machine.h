#ifndef MAGICCUP_INTERNAL_STATE_MACHINE_H
#define MAGICCUP_INTERNAL_STATE_MACHINE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

namespace magiccup {

///
/// @enum EGameState
/// @brief Game status enumeration for SceneGamePlay scene.
///
enum class EGameState {
  None,
  Initialize,
  GameStart,
  Shaking,
  Select,
  Result,
  GameOver,
  NextStage,
  GameEnd
};

} /// ::magiccup namespace

#endif /// MAGICCUP_INTERNAL_STATE_MACHINE_H