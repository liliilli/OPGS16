
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_title.h"

#include <Component/sprite2d_renderer.h>
#include <Element/object.h>
#include <Element/canvas/canvas.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Object/CanvasItem/background.h"
#include "../../../Include/Script/CanvasItem/script_background.h"

namespace magiccup {

void ScriptTitleDisplay::Initiate() {
  auto& bind_obj = GetBindObject();

  using opgs16::element::canvas::CImage;
  using opgs16::element::canvas::CCanvas;
  m_background = bind_obj.CreateGameObject<UiBackground>(
      UiBackground::s_object_name,
      static_cast<CCanvas*>(&bind_obj))->GetComponent<ScriptUiBackground>();
  m_background->SetScale(0.25f, -0.25f);

  OP16_TIMER_SET(m_temporary_timer, 1'000, true, this,
                 &ScriptTitleDisplay::ChangeBackground);
}

void ScriptTitleDisplay::ChangeBackground() {
  m_background_index += 1;
  m_background_index %= 6;

  m_background->SetTile(m_background_index);
}

} /// ::magiccup namespace
