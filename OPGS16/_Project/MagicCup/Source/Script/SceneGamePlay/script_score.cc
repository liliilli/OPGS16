
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_score.h"

#include <Component/empty_renderer.h>
#include <Element/Canvas/text.h>

namespace magiccup {

void ScriptUiScore::Initiate() {
  using opgs16::element::canvas::CText;
  auto& obj = GetBindObject();

  auto text = obj.Instantiate<CText>("Text", "Score");
  text->SetOrigin(IOriginable::Origin::UP_LEFT);
  text->SetFontName("opSystem");
  text->SetFontSize(8);
  text->SetWorldPosition({16.f, -24.f, 0});
  text->GetComponent<opgs16::component::CEmptyRenderer>()->SetRenderLayer(3);

  auto score = obj.Instantiate<CText>("Score", "00000");
  score->SetOrigin(IOriginable::Origin::UP_LEFT);
  score->SetWorldPosition({16.f, -32.f, 0.f});
  score->SetFontName("opSystem");
  score->SetFontSize(8);
  score->GetComponent<opgs16::component::CEmptyRenderer>()->SetRenderLayer(3);

  m_score = score;
}

void ScriptUiScore::UpdateScore(int32_t score) noexcept {
  m_score->SetText(std::to_string(score));
}

} /// ::magiccup namespace