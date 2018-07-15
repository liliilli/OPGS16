
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_timelimit.h"

#include <Component/empty_renderer.h>
#include <Component/sprite2d_renderer.h>
#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/SceneGamePlay/timer_image.h"
#include "../../../Include/Internal/object_keyword.h"

namespace magiccup {

void ScriptUiTimelimit::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;

  auto text = obj.CreateGameObject<CText>("Text", "Time");
  text->SetAlignment(IAlignable::Alignment::CENTER);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({0, 20.f, 0});
  text->GetComponent<opgs16::component::CEmptyRenderer>()->SetRenderingLayer(3);

  using opgs16::manager::scene::GetPresentScene;
  if (auto canvas = GetPresentScene()->GetGameObject(name::canvas);
      canvas) {
    auto raw_ptr = static_cast<opgs16::element::canvas::CCanvas*>(canvas);
    auto timer = obj.CreateGameObject<TimerImage>("Timer", raw_ptr);
    timer->SetWorldPosition({0.f, 8.f, 0.f});
    timer->GetComponent<opgs16::component::CSprite2DRenderer>()->SetRenderingLayer(3);
  }
}

} /// ::magiccup namespace