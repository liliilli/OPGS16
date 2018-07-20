
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_component.h"

#include <Element/object.h>
#include <Element/canvas/canvas.h>
#include <Element/canvas/text.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Internal/object_keyword.h"
#include "../../../Include/Object/CanvasItem/background.h"
#include "../../../Include/Script/CanvasItem/script_background.h"

namespace magiccup {

void ScriptTitleComponent::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CCanvas;
  using opgs16::element::canvas::CText;
  using opgs16::manager::scene::GetPresentScene;

  m_canvas = static_cast<CCanvas*>(GetPresentScene()->GetGameObject(name::canvas));

  auto subject = obj.CreateGameObject<CText>(
      "Subject",
      "OPGS16 0.1.0 version sample game");
  subject->SetFontName("Font1");
  subject->SetFontSize(8);
  subject->SetAlignment(IAlignable::Alignment::CENTER);
  subject->SetWorldPosition({0, 16.f, 0});
  subject->SetObjectActive(false);
  m_subject = subject;

  auto text = obj.CreateGameObject<CText>(
      "Copyright",
      "2018 Jongmin Yun(Neu), ALL RIGHTS RESERVED");
  text->SetColor(opgs16::DColor::Gray);
  text->SetFontName("Font1");
  text->SetFontSize(8);
  text->SetAlignment(IAlignable::Alignment::RIGHT);
  text->SetOrigin(IOriginable::Origin::DOWN_RIGHT);
  text->SetWorldPosition({-16.f, 12.f, 0});
  text->SetObjectActive(false);
  m_copyright = text;

  SetComponentActive(false);
}

void ScriptTitleComponent::EnableComponent() {
  m_subject->SetObjectActive(true);
  m_copyright->SetObjectActive(true);
}

} /// ::magiccup namespace