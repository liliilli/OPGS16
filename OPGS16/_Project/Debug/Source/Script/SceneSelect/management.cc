
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
#include <Manager/scene_manager.h>

#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/SceneSelect/select_manager.h"

#include "../../../Include/Scene/Test/object_lifecycle.h"
#include "../../../Include/Scene/Test/random.h"
#include "../../../Include/Scene/Test/sound.h"

namespace {

std::vector<std::string_view> description_container = {
  u8"�� �׽�Ʈ��\n������Ʈ �����ֱ⸦\n�׽�Ʈ�մϴ�.",
  u8"���� �� �����\n�׽�Ʈ�մϴ�.",
  u8"BGM, Effect ����\n���带 �׽�Ʈ�մϴ�.",
  u8"Ű����, ���콺 ����\n��ǲ ����� �׽�Ʈ�մϴ�.",
  u8"There is nothing!",
  u8"There is nothing!",
  u8"There is nothing!",
  u8"There is nothing!"
};

} /// unnamed namespace

namespace debug::script {

void Management::Initiate() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto& obj = GetBindObject();

  auto subject = obj.Instantiate<CText>("Subject", "OPGS16 Test");
  subject->SetFontName("Bios");
  subject->SetWorldPosition({0.f, -24.f, 0.f});
  subject->SetFontSize(16);
  subject->SetOrigin(IOriginable::Origin::UP_CENTER);
  subject->SetAlignment(IAlignable::Alignment::CENTER);

  auto list = obj.Instantiate<ChoiceList>(
      object::SelectManager::list_name, "opSystem",
      static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject()),
      std::vector<std::string>{
          "Object lifecycle test", "Random feature test",
          "Sound test",
          "Input test",
          "Rendering test.",
          "Nothing.", "Nothing.", "Nothing."
      }
  );
  list->SetItemSize(12);
  list->SetFontSize(8);
  list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  list->SetTextureIndex(8);
  list->SetSelectedColor(opgs16::DColor::Aqua);
  list->SetNormalColor(opgs16::DColor::Gray);
  list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  list->SetWorldPosition({48.f, 96.f, 0.f});
  list->SetFunction(0, std::bind(&Management::ExecuteLifecycleTest, this));
  list->SetFunction(1, std::bind(&Management::ExecuteRandomFeatureTest, this));
  list->SetFunction(2, std::bind(&Management::ExecuteSoundTest, this));
  m_list = list;

  auto copyright = obj.Instantiate<CText>("Copyright", "2018 Jongmin Yun(Neu.)");
  copyright->SetFontName("opSystem");
  copyright->SetFontSize(8);
  copyright->SetColor(opgs16::DColor::Gray);
  copyright->SetOrigin(IOriginable::Origin::DOWN_RIGHT);
  copyright->SetAlignment(IAlignable::Alignment::RIGHT);
  copyright->SetWorldPosition({-8.f, 8.f, 0.f});

  auto description = obj.Instantiate<object::Description>("Description");
  m_description = description;
  m_description->SetText(description_container[m_list->GetCursorIndex()].data());
}

void Management::Start() { }

void Management::Update(float delta_time) {
  Input();
}

void Management::Input() {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;

  if (m_is_pressed == true) {
    if (IsKeyReleased("Vertical")) m_is_pressed = false;
    return;
  }

  auto key_val = GetKeyValue("Vertical");
  if (key_val == 1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Up);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_container[new_idx].data());
    }
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Down);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_container[new_idx].data());
    }
    m_is_pressed = true;
  }

  if (IsKeyPressed("Horizontal")) {
    m_list->SelectCommand();
  }
}

void Management::ExecuteLifecycleTest() {
  M_REPLACE_SCENE(scene::ObjectLifeCycle);
}

void Management::ExecuteRandomFeatureTest() {
  M_REPLACE_SCENE(scene::RandomTestScene);
}

void Management::ExecuteSoundTest() {
  M_REPLACE_SCENE(scene::SoundTestScnee);
}

} /// ::debug::script namespace