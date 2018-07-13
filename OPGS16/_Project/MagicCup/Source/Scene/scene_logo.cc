
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../Include/Scene/scene_logo.h"

#include "../../Include/Object/Common/empty_canvas.h"
#include "../../Include/Internal/object_keyword.h"
#include "../../Include/Script/SceneLogo/script_logo.h"

namespace magiccup {

void SceneLogo::Initiate() {
  this->SetBackgroundColor(opgs16::DColor::Magenta);

  auto canvas = Instantiate<magiccup::EmptyCanvas>(name::canvas);
  canvas->AddComponent<ScriptLogo>(*canvas);
}

} /// ::magiccup namespace