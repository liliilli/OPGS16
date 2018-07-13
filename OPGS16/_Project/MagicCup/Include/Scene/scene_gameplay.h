#ifndef MAGICCUP_SCENE_SCENE_PLAY_H
#define MAGICCUP_SCENE_SCENE_PLAY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/scene.h>

namespace magiccup {

class SceneGamePlay final : public opgs16::element::CScene {
  void Initiate() override;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_SCENE_SCENE_PLAY_H