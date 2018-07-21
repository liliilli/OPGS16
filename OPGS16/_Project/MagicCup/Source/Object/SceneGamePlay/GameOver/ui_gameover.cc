
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Gameover/ui_gameover.h"

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

UiGameOverObject::UiGameOverObject(opgs16::element::canvas::CCanvas* canvas) :
    CImage{keyword::rsc_sprite, canvas} {
  SetRenderingLayer("Foremost");
  SetTextureFragmentIndex(9);
  SetImageSize(144, 32);
  SetOrigin(Origin::UP_CENTER);
}

} /// ::magiccup namespace