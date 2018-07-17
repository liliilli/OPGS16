
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/timer_image.h"

namespace magiccup {

TimerImage::TimerImage(const opgs16::element::canvas::CCanvas* canvas_reference) :
    CImage{"opSystem", canvas_reference} {
  SetTextureIndex(0);
  SetLocalPosition({0.f, 4.f, 0.f});
  SetRenderingLayer(3);
}

} /// ::magiccup namespace