
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
#include <Manager/sound_manager.h>

#include <Component/font2d_renderer.h>
#include <Component/sprite2d_renderer.h>

#include "../../../Include/Object/SceneGamePlay/life_image.h"
#include "../../../Include/Internal/object_keyword.h"
#include "../../../Include/Object/SceneGamePlay/effect_boom_object.h"

namespace magiccup {

void ScriptUiLife::Initiate() {
  using opgs16::element::canvas::CText;
  using opgs16::element::canvas::CCanvas;
  using opgs16::manager::scene::GetPresentScene;

  auto& obj = GetBindObject();

  auto text = obj.CreateGameObject<CText>("Text", "Life");
  text->SetOrigin(IOriginable::Origin::UP_RIGHT);
  text->SetAlignment(IAlignable::Alignment::RIGHT);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({-16.f, -24.f, 0});
  text->SetRenderingLayer(3);

  if (auto canvas = GetPresentScene()->GetGameObject(name::canvas)) {
    auto raw_ptr = static_cast<CCanvas*>(canvas);

    for (int32_t i = 1; i <= 5; ++i) {
      auto instance = obj.CreateGameObject<LifeImage>("_", raw_ptr);
      instance->SetWorldPosition({-16.f * i, -24.f, 0});
      instance->SetRenderingLayer(3);
      instance->SetObjectActive(false);
      m_life_container.push_back(instance);
    }
  }
}

void ScriptUiLife::UpdateLife(int32_t life) noexcept {
  using opgs16::manager::sound::PlaySound;
  using opgs16::manager::scene::GetPresentScene;

  const auto sep = (life <= 5 ? life : 5);

  int32_t present_life = 0;
  for (const auto& life_elem : m_life_container) {
    if (life_elem->IsObjectActive() == phitos::enums::EActivated::Activated) {
      ++present_life;
    }
  }

  if (present_life > life) {
    const auto step = present_life - life;
    for (int32_t i = life; i < present_life; ++i) {
      const auto& position = m_life_container[i]->GetFinalPosition();
      auto obj = GetPresentScene()->CreateGameObject<EffectBoomObject>("Boom");
      obj->SetWorldPosition(position);
    }
  }

  for (int32_t i = 0; i < sep; ++i) {
    m_life_container[i]->SetObjectActive(true);
  }

  for (int32_t i = life; i < 5; ++i) {
    m_life_container[i]->SetObjectActive(false);
  }
}

} /// ::magiccup namespace