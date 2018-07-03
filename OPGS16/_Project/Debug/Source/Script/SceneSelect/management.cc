
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SceneSelect/management.h"

#include <Element/object.h>
#include <Element/Canvas/text.h>
#include <Helper/Type/vectori2.h>
#include <Manager/input_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/SceneSelect/choice_list.h"
#include "../../../Include/Object/SceneSelect/select_manager.h"

namespace debug::script {

void Management::Initiate() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto& obj = GetBindObject();

  auto subject = obj.Instantiate<CText>("Subject", "Debug Test");
  subject->SetFontName("opSystem");
  subject->SetWorldPosition({32.f, -32.f, 0.f});
  subject->SetFontSize(16);
  subject->SetOrigin(IOriginable::Origin::UP_CENTER);
  subject->SetAlignment(IAlignable::Alignment::CENTER);

  auto list = obj.Instantiate<ChoiceList>(
      object::SelectManager::list_name, "opSystem",
      static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject()),
      std::vector<std::string>{
      "Object lifecycle test", "Random feature test",
      "Nothing.", "Nothing.", "Nothing.", "Nothing.", "Nothing.", "Nothing."});
  list->SetItemSize(12);
  list->SetFontSize(8);
  list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  list->SetSelectedColor(opgs16::DColor{0.0f, 1.0f, 1.0f});
  list->SetNormalColor(opgs16::DColor{.5f, .5f, .5f});
  list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  list->SetWorldPosition({48.f, 96.f, 0.f});
  list->SetFunction(0, std::bind(&Management::ExecuteLifecycleTest, this));
  list->SetFunction(1, std::bind(&Management::ExecuteRandomFeatureTest, this));
  m_list = list;

  auto copyright = obj.Instantiate<CText>("Copyright", "2018 Jongmin Yun(Neu.)");
  copyright->SetFontName("opSystem");
  copyright->SetFontSize(8);
  copyright->SetColor(opgs16::DColor{0.5f, 0.5f, 0.5f});
#ifdef false
  copyright->SetOrigin(Origin::DOWN_RIGHT);
  copyright->SetAlignment(IAlignable::Alignment::RIGHT);
  copyright->SetWorldPosition({-8.f, 8.f, 0.f});
#endif
  copyright->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  copyright->SetAlignment(IAlignable::Alignment::LEFT);
  copyright->SetWorldPosition({8.f, 8.f, 0.f});

  auto description = obj.Instantiate<CText>("Description", "This is description.");
  description->SetFontName("opSystem");
  description->SetFontSize(8);
  description->SetColor(opgs16::DColor{1.f, 1.f, 1.f});
  description->SetOrigin(IOriginable::Origin::DOWN_CENTER);
  description->SetAlignment(IAlignable::Alignment::CENTER);
  description->SetWorldPosition({0.f, 48.f, 0.f});
}

void Management::Start() {

}

void Management::Update(float delta_time) {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;

  if (m_is_pressed == true) {
    if (IsKeyReleased("Vertical")) m_is_pressed = false;
    return;
  }

  auto key_val = GetKeyValue("Vertical");
  if (key_val == 1.0f) {
    m_list->MoveCursor(object::EDirection::Up);
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    m_list->MoveCursor(object::EDirection::Down);
    m_is_pressed = true;
  }

  if (IsKeyPressed("Horizontal")) {
    m_list->SelectCommand();
  }
}

  void Management::ExecuteLifecycleTest()
  {
    PHITOS_NOT_IMPLEMENTED_ASSERT();
  }

  void Management::ExecuteRandomFeatureTest()
  {
    PHITOS_NOT_IMPLEMENTED_ASSERT();
  }
} /// ::debug::script namespace