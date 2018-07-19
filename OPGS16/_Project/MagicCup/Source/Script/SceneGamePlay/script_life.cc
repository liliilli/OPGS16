
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_life.h"

#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>

#include <Component/font2d_renderer.h>
#include <Component/sprite2d_renderer.h>

#include "../../../Include/Object/SceneGamePlay/life_image.h"
#include "../../../Include/Internal/object_keyword.h"

namespace magiccup {

void ScriptUiLife::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;

  auto text = obj.CreateGameObject<CText>("Text", "Life");
  text->SetOrigin(IOriginable::Origin::UP_RIGHT);
  text->SetAlignment(IAlignable::Alignment::RIGHT);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({-16.f, -24.f, 0});
  text->GetComponent<opgs16::component::CFont2DRenderer>()->SetRenderingLayer(3);

  using opgs16::manager::scene::GetPresentScene;
  if (auto canvas = GetPresentScene()->GetGameObject(name::canvas);
      canvas) {
    auto raw_ptr = static_cast<opgs16::element::canvas::CCanvas*>(canvas);

    for (int32_t i = 1; i <= 5; ++i) {
      auto instance = obj.CreateGameObject<LifeImage>("_", raw_ptr);
      instance->SetWorldPosition({-16.f * i, -24.f, 0});
      instance->GetComponent<opgs16::component::CSprite2DRenderer>()->
          SetRenderingLayer(3);
      m_life_container.push_back(instance);
    }
  }
}

void ScriptUiLife::UpdateLife(int32_t life) noexcept {
  const auto sep = (life <= 5 ? life : 5);
  for (int32_t i = 0; i < sep; ++i) {
    m_life_container[i]->SetObjectActive(true);
  }

  for (int32_t i = life; i < 5; ++i) {
    m_life_container[i]->SetObjectActive(false);
  }
}

} /// ::magiccup namespace