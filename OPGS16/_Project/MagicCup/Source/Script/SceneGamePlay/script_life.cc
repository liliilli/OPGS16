
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

#include "../../../Include/Object/SceneGamePlay/life_image.h"
#include "../../../Include/Internal/object_keyword.h"

namespace magiccup {

void ScriptUiLife::Initiate() {
  auto& obj = GetBindObject();
  using opgs16::element::canvas::CText;

  auto text = obj.Instantiate<CText>("Text", "Life");
  text->SetOrigin(IOriginable::Origin::UP_RIGHT);
  text->SetAlignment(IAlignable::Alignment::RIGHT);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({-16.f, -24.f, 0});

  auto& canvas =
      opgs16::manager::scene::GetPresentScene()->GetGameObject(name::canvas);
  if (canvas) {
    auto raw_ptr = static_cast<opgs16::element::canvas::CCanvas*>(canvas.get());

    for (int32_t i = 1; i <= 5; ++i) {
      auto instance = obj.Instantiate<LifeImage>("_", raw_ptr);
      instance->SetWorldPosition({-16.f * i, -24.f, 0});
      m_life_container.push_back(instance);
    }
  }
}

void ScriptUiLife::UpdateLife(int32_t life) noexcept {

}

} /// ::magiccup namespace