
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneMain/ui_background_part.h"

#include <Element/Canvas/canvas.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/CanvasItem/background.h"
#include "../../../Include/Script/CanvasItem/script_background.h"

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Internal/object_keyword.h"
#include "../../../Include/Script/SceneMain/script_background_part.h"

namespace magiccup {

UiBackgroundPart::UiBackgroundPart() {
  using opgs16::manager::scene::GetPresentScene;
  using opgs16::element::canvas::CCanvas;

  SetScaleValue(0.5f);
  SetScaleFactor({108.f, 208.f, 0.f});

  auto m_canvas = static_cast<CCanvas*>(GetPresentScene()->GetGameObject(name::canvas));
  CreateGameObject<UiBackground>(UiBackground::s_object_name, m_canvas);
  this->AddComponent<ScriptBackgroundPart>(*this);

  this->SetObjectActive(false);
}

} /// ::magiccup namespace