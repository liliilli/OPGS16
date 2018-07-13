
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

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Scene/scene_gameplay.h"

namespace magiccup {

void ScriptTitleSelect::Initiate() {
  auto& obj = GetBindObject();
  auto choice_list = obj.Instantiate<ChoiceList>("ChoiceList",
    "opSystem",
    static_cast<opgs16::element::canvas::CCanvas*>(&obj),
    std::vector<std::string>{"New Game", "Exit"});
  choice_list->SetCursorSize({16, 16});
  choice_list->SetFontSize(8);
  choice_list->SetItemSize(12);
  choice_list->SetSelectedColor(opgs16::DColor::White);
  choice_list->SetNormalColor(opgs16::DColor::Gray);
  choice_list->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  choice_list->SetWorldPosition({-24.f, 48.f, 0.f});
  choice_list->SetTextureIndex(8);

  choice_list->SetFunction(0, std::bind(&ScriptTitleSelect::GotoGameplay, this));
  choice_list->SetFunction(1, std::bind(&ScriptTitleSelect::ExitGame, this));

  m_choice_list = choice_list;
}

void ScriptTitleSelect::Update(float delta_time) {
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::GetKeyValue;

  const auto up_value = GetKeyValue(keyword::key_y);
  if (up_value == 1.0f) {
    m_choice_list->MoveCursor(EDirection::Up);
  }
  else if (up_value == -1.0f) {
    m_choice_list->MoveCursor(EDirection::Down);
  }

  if (IsKeyPressed(keyword::key_enter)) {
    m_choice_list->SelectCommand();
  }
}

void ScriptTitleSelect::GotoGameplay() {
  M_REPLACE_SCENE(magiccup::SceneGamePlay);
}

void ScriptTitleSelect::ExitGame() {

}

void ScriptTitleSelect::Destroy() { }

} /// ::magiccup namespace