
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Script/SceneGamePlay/GameOver/script_nameentry.h"

#include <Element/object.h>
#include <Manager/input_manager.h>
#include <Manager/timer_manager.h>

#include "../../../../Include/Object/SceneGamePlay/Gameover/ui_char.h"
#include "../../../../Include/Script/SceneGamePlay/GameOver/script_char.h"

#include "../../../../Include/Global/data_singleton.h"
#include "../../../../Include/Internal/general_keyword.h"
#include "../../../../Include/Scene/scene_main.h"
#include "Manager/scene_manager.h"

namespace magiccup {

void ScriptNameEntry::Initiate() {
  auto& obj = GetBindObject();

  auto chr = obj.CreateGameObject<UiCharRouletteObject>(UiCharRouletteObject::s_gameobject_name);
  chr->SetWorldPosition({-24.f, 0, 0});
  m_characters[0] = chr->GetComponent<ScriptCharRoulette>();

  chr = obj.CreateGameObject<UiCharRouletteObject>(UiCharRouletteObject::s_gameobject_name);
  chr->SetWorldPosition({0, 0, 0});
  m_characters[1] = chr->GetComponent<ScriptCharRoulette>();

  chr = obj.CreateGameObject<UiCharRouletteObject>(UiCharRouletteObject::s_gameobject_name);
  chr->SetWorldPosition({24.f, 0, 0});
  m_characters[2] = chr->GetComponent<ScriptCharRoulette>();
}

void ScriptNameEntry::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::GetKeyValue;

  if (IsKeyPressed(keyword::key_y)) {
    if (const auto key_val = GetKeyValue(keyword::key_y);
        key_val == 1.0f) {
      m_characters[m_focus_index]->UpChar();
    }
    else if (key_val == -1.0f) {
      m_characters[m_focus_index]->DownChar();
    }
  }

  if (IsKeyPressed(keyword::key_enter)) {
    if (m_focus_index >= 2) {
      // Enter!
      std::string result_name;
      for (const auto& element : m_characters) {
        result_name += element->GetCharacter();
      }

      SetComponentActive(false);
      const int32_t index = data::GetRecentDataRank();
      data::LoadRankData(index).name = result_name;

      OP16_TIMER_SET(m_return_timer, 1'000, false, this,
                     &ScriptNameEntry::CbReturnToMainScene);
      return;
    }

    m_characters[m_focus_index]->SetComponentActive(false);
    m_characters[m_focus_index]->DecideCharacter();
    m_focus_index += 1;
  }
}

void ScriptNameEntry::CbReturnToMainScene() {
  data::SaveSaveFile();
  data::LoadSaveFile();
  M_REPLACE_SCENE(SceneMain);
}

} /// ::magiccup namespace