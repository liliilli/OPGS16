
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/life_image.h"

namespace magiccup {

LifeImage::LifeImage(const opgs16::element::canvas::CCanvas* canvas_reference) :
    CImage{ "opSystem", canvas_reference} {
  SetImageSize(16.f, 16.f);
  SetLocalPosition({-8.f, -8.f, 0});
}

} /// ::magiccup namespace