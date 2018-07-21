#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_GAMEOVER_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_GAMEOVER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/image.h>

namespace magiccup {

class UiGameOverObject final : public opgs16::element::canvas::CImage {
public:
  UiGameOverObject(opgs16::element::canvas::CCanvas* canvas);

  inline static constexpr const char* s_object_name = "gameover_disp";
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_GAMEOVER_UI_GAMEOVER_H