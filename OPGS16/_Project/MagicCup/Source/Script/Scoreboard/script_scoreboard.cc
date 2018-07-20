
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/Scoreboard/script_scoreboard.h"

#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Helper/Type/vectori2.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Internal/object_keyword.h"

#include "../../../Include/Object/Common/ui_tile9_dialogue_frame.h"
#include "../../../Include/Script/Common/script_tile9_frame.h"

#include "../../../Include/Global/data_singleton.h"
#include "../../../Include/Object/Scoreboard/ui_rank_text.h"

namespace magiccup {

void ScriptScoreboard::Initiate() {
  using opgs16::manager::scene::GetPresentScene;
  auto& obj = GetBindObject();

  auto tile = obj.CreateGameObject<UiTile9DialogueFrame>(
      UiTile9DialogueFrame::s_shader_name,
      keyword::rsc_sprite,
      static_cast<opgs16::element::canvas::CCanvas*>(GetPresentScene()->GetGameObject(name::canvas)));

  tile->SetInterval(500);
  tile->SetEdgeImageSize(16);
  tile->SetFullImageSize(opgs16::DVectorInt2{192, 208});
  tile->SetTextureFragmentIndex(11);
  tile->SetAlpha(0.5f);
  tile->Execute(true);
  m_frame = tile->GetScript();
}

void ScriptScoreboard::Update(float delta_time) {
  using opgs16::element::canvas::CText;

  if (m_frame->IsExecutionDone()) {
    auto& obj = GetBindObject();

    const auto& container = data::LoadRankDataContainer();
    const int32_t size = static_cast<int32_t>(container.size());

    for (int32_t i = 0; i < size; ++i) {
      auto text = obj.CreateGameObject<UiRankText>("_");
      text->SetTextContainer(i + 1, container[i]);
      text->SetWorldPosition({0.f, -48.f - (12.f * i), 0.f});
      m_text[i] = text;
    }

    this->SetComponentActive(false);
  }
}

void ScriptScoreboard::ExecuteVanishScoreboard() {

}

void ScriptScoreboard::Destroy() {

}

} /// ::magiccup namespace