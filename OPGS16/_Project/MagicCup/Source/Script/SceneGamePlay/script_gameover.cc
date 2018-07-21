
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneGamePlay/script_gameover.h"

#include <Element/object.h>
#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Manager/scene_manager.h>
#include <Manager/timer_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Global/data_singleton.h"
#include "../../../Include/Object/SceneGamePlay/Gameover/ui_gameover.h"
#include "../../../Include/Object/SceneGamePlay/Gameover/ui_nameentry.h"
#include "../../../Include/Scene/scene_main.h"
#include "../../../Include/Script/Scoreboard/rank_text_divingin.h"

namespace magiccup {

void ScriptGameOverSequence::Initiate() {
  using opgs16::element::canvas::CCanvas;
  using opgs16::element::canvas::CText;

  auto& obj = GetBindObject();

  auto gameover = obj.CreateGameObject<UiGameOverObject>(
      UiGameOverObject::s_object_name,
      static_cast<CCanvas*>(&obj)
  );
  gameover->SetWorldPosition({0.f, -320.f, 0.f});
  {
    auto eff = gameover->AddComponent<ScriptRankTextDivingIn>(*gameover);
    eff->ExecuteMovingEffect(100, 500, opgs16::DVector3{0.f, -64.f, 0.f});
  }

  if (auto index = data::GetRecentDataRank(); index != -1) {
    std::string rank_string = "You ranked in ";
    rank_string.append(std::to_string(index + 1));
    switch (index + 1) {
    case 1: rank_string.append("st!"); break;
    case 2: rank_string.append("nd!"); break;
    case 3: rank_string.append("rd!"); break;
    default: rank_string.append("th!"); break;
    }
    auto text = obj.CreateGameObject<CText>("Statement", rank_string);
    text->SetFontName("opSystem");
    text->SetFontSize(8);
    text->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    text->SetAlignment(IAlignable::Alignment::CENTER);
    text->SetRenderingLayer("Foremost");
    text->SetWorldPosition({0.f, -320.f, 0.f});

    {
      auto eff = text->AddComponent<ScriptRankTextDivingIn>(*text);
      eff->ExecuteMovingEffect(300, 500, opgs16::DVector3{0.f, 96.f, 0.f});
    }

    auto entry = obj.CreateGameObject<UiNameEntryObject>("Entry");
    entry->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    entry->SetWorldPosition({0.f, 64.f, 0.f});
  }
  else {
    auto text = obj.CreateGameObject<CText>(
        "Statement",
        "You did not rank in 10th..\ntry again!");
    text->SetFontName("opSystem");
    text->SetFontSize(8);
    text->SetOrigin(IOriginable::Origin::DOWN_CENTER);
    text->SetAlignment(IAlignable::Alignment::CENTER);
    text->SetRenderingLayer("Foremost");
    text->SetWorldPosition({0.f, -320.f, 0.f});

    {
      auto eff = text->AddComponent<ScriptRankTextDivingIn>(*text);
      eff->ExecuteMovingEffect(300, 500, opgs16::DVector3{0.f, 64.f, 0.f});
    }

    OP16_TIMER_SET(m_return_timer, 3'000, false, this,
                   &ScriptGameOverSequence::CbReturnToMainScene);
  }

  this->SetComponentActive(false);
}

void ScriptGameOverSequence::CbReturnToMainScene() {
  data::SaveSaveFile();
  data::LoadSaveFile();
  M_REPLACE_SCENE(SceneMain);
}

void ScriptGameOverSequence::Update(float delta_time) { }

} /// ::magiccup namespace