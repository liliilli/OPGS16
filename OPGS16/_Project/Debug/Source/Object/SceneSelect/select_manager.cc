
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneSelect/select_manager.h"

#include <Element/Canvas/text.h>
#include <Helper/Type/vectori2.h>

#include "../../../Include/Object/SceneSelect/choice_list.h"
#include "../../../Include/Script/SceneSelect/management.h"

namespace debug::object {

SelectManager::SelectManager() : CCanvas() {
  using opgs16::element::canvas::CText;
  auto subject = Instantiate<CText>("Subject", "Debug Test");
  subject->SetFontName("opSystem");
  subject->SetWorldPosition({32.f, -32.f, 0.f});
  subject->SetFontSize(16);
  subject->SetOrigin(IOriginable::Origin::UP_CENTER);
  subject->SetAlignment(IAlignable::Alignment::CENTER);

  auto list = Instantiate<ChoiceList>(list_name, "opSystem", this,
    std::vector<std::string>{
    "Hello",
    "World D:",
    "Bye",
    "World :D"
  });
  list->SetItemSize(16);
  list->SetFontSize(8);
  list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  list->SetSelectedColor(opgs16::DColor{0.0f, 1.0f, 1.0f});
  list->SetOrigin(Origin::DOWN_LEFT);
  list->SetWorldPosition({64.f, 64.f, 0.f});

  auto script = AddComponent<script::Management>(*this);
}

} /// ::debug::object namespace