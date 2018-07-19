
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/CanvasItem/background.h"

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Script/CanvasItem/script_background.h"

namespace magiccup {

UiBackground::UiBackground(opgs16::element::canvas::CCanvas* canvas_object) :
    CImage{keyword::rsc_sprite, canvas_object} {
  SetImageSize(256, 256);
  SetTextureFragmentIndex(0);
  SetRenderingLayer(0);

  AddComponent<ScriptUiBackground>(*this);
} /// ::magiccup namespace

}
