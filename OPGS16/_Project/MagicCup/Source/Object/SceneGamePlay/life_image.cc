
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/life_image.h"

#include "../../../Include/Internal/general_keyword.h"

namespace magiccup {

LifeImage::LifeImage(const opgs16::element::canvas::CCanvas* canvas_reference) :
    CImage{ keyword::rsc_sprite, canvas_reference} {
  SetImageSize(32.f, 32.f);
  SetTextureFragmentIndex(7);
  SetLocalPosition({-8.f, -8.f, 0});
}

} /// ::magiccup namespace