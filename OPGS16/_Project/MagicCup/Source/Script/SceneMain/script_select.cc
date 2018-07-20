
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneMain/script_select.h"

#include <Element/Canvas/canvas.h>
#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>
#include <Manager/sound_manager.h>
#include <Manager/timer_manager.h>

#include <Core/application.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/fade_out.h"
#include "../../../Include/Scene/scene_gameplay.h"
#include "../../../Include/Internal/object_keyword.h"

namespace magiccup {

void ScriptTitleSelect::Initiate() {
  using opgs16::manager::scene::GetPresentScene;
  using opgs16::element::canvas::CCanvas;

  const auto canvas = GetPresentScene()->GetGameObject(name::canvas);
  auto& obj = GetBindObject();

  auto choice_list = obj.CreateGameObject<ChoiceList>(
      "ChoiceList",
      "opSystem",
      static_cast<CCanvas*>(canvas),
      std::vector<std::string>{"New Game", "Exit"});
  choice_list->SetCursorSize({16, 16});
  choice_list->SetFontSize(8);
  choice_list->SetItemSize(12);
  choice_list->SetSelectedColor(opgs16::DColor::White);
  choice_list->SetNormalColor(opgs16::DColor::Gray);
  choice_list->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  choice_list->SetWorldPosition({-24.f, 48.f, 0.f});
  choice_list->SetTextureIndex(8);
  choice_list->SetRenderingLayer(3);

  choice_list->SetFunction(0, std::bind(&ScriptTitleSelect::GotoGameplay, this));
  choice_list->SetFunction(1, std::bind(&ScriptTitleSelect::ExitGame, this));

  using opgs16::manager::sound::GenerateSound;
  GenerateSound(keyword::eff_cursormove);
  GenerateSound(keyword::eff_decision);
  GenerateSound(keyword::eff_exit);

  m_choice_list = choice_list;
  m_choice_list->SetObjectActive(false);
  SetComponentActive(false);
}

void ScriptTitleSelect::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::sound::PlaySound;

  const auto up_value = GetKeyValue(keyword::key_y);
  if (IsKeyPressed(keyword::key_y)) {
    if (up_value == 1.0f) {
      m_choice_list->MoveCursor(EDirection::Up);
      PlaySound(keyword::eff_cursormove);
    }
    else if (up_value == -1.0f) {
      m_choice_list->MoveCursor(EDirection::Down);
      PlaySound(keyword::eff_cursormove);
    }
  }

  if (IsKeyPressed(keyword::key_enter)) {
    m_choice_list->SelectCommand();
  }
}

void ScriptTitleSelect::GotoGameplay() {
  using opgs16::manager::sound::PlaySound;
  PlaySound(keyword::eff_decision);

  OP16_TIMER_SET(m_common_timer, 1'000, false, this,
                 &ScriptTitleSelect::ExecuteGotoGamePlay);
}

void ScriptTitleSelect::ExecuteGotoGamePlay() {
  M_REPLACE_SCENE(magiccup::SceneGamePlay);
}

void ScriptTitleSelect::ExitGame() {
  using opgs16::manager::sound::PlaySound;
  using opgs16::manager::scene::GetPresentScene;
  using opgs16::element::canvas::CCanvas;

  PlaySound(keyword::eff_exit);

  const auto canvas = GetPresentScene()->GetGameObject(name::canvas);
  auto& obj = GetBindObject();

  obj.CreateGameObject<UiFadeOut>(UiFadeOut::s_object_name,
      1'000,
      std::bind(&ScriptTitleSelect::ExecuteExit, this),
      static_cast<CCanvas*>(canvas));
}

void ScriptTitleSelect::ExecuteExit() {
  opgs16::entry::ExitGame();
}

void ScriptTitleSelect::Destroy() {
  using opgs16::manager::sound::DestroySound;
  DestroySound(keyword::eff_cursormove);
  DestroySound(keyword::eff_decision);
  DestroySound(keyword::eff_exit);
}

void ScriptTitleSelect::EnableComponent() {
  m_choice_list->SetObjectActive(true);
  SetComponentActive(true);
}

} /// ::magiccup namespace