
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/ObjectTest/obj_test_mgn_script.h"

#include <Element/Canvas/canvas.h>
#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/copyright.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/Common/gotoback.h"
#include "../../../Include/Object/Common/subject.h"

#include "../../../Include/Internal/keyword.h"

#include "../../../Include/Scene/select_scene.h"
#include "../../../Include/Scene/ObjectTest/tree_activation.h"
#include "../../../Include/Scene/Test/object_lifecycle.h"

namespace {

std::vector<std::string> command_list = {
  "Lifecycle Test", "Tree Activation Test"
};

std::vector<std::string> description_list = {
  u8"오브젝트 생명주기를\n테스트 합니다.",
  u8"오브젝트의 활성화를\n테스트 합니다."
};

} /// unnamed namespace

namespace debug::script {

void ObjectTestManagerScript::Initiate() {
  m_obj = static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject());
  m_obj->Instantiate<object::GotoBackSign>("Back");
  m_obj->Instantiate<object::Copyright>("Copyright");

  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto subject = m_obj->Instantiate<object::Subject>("Subject");
  subject->SetText("GameObject feature test");
  m_subject = subject;

  m_list = m_obj->Instantiate<ChoiceList>(
      "CommandList", "opSystem", m_obj, command_list);
  m_list->SetItemSize(12);
  m_list->SetFontSize(8);
  m_list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  m_list->SetTextureIndex(8);
  m_list->SetSelectedColor(opgs16::DColor::Aqua);
  m_list->SetNormalColor(opgs16::DColor::Gray);
  m_list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  m_list->SetWorldPosition({32.f, 96.f, 0.f});

  m_list->SetFunction(0, std::bind(&ObjectTestManagerScript::ExecuteLifecycleTest, this));
  m_list->SetFunction(1, std::bind(&ObjectTestManagerScript::ExecuteActivationTest, this));

  m_description = m_obj->Instantiate<object::Description>("Desc");
  m_description->SetText(description_list[m_list->GetCursorIndex()]);
}

void ObjectTestManagerScript::Update(float delta_time) {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;

  if (m_is_pressed == true) {
    if (IsKeyReleased(keyword::key_axis_vert)) m_is_pressed = false;
    return;
  }

  const auto key_val = GetKeyValue(keyword::key_axis_vert);
  if (key_val == 1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Up);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_list[new_idx]);
    }
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Down);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_list[new_idx]);
    }
    m_is_pressed = true;
  }

  if (IsKeyPressed(keyword::key_enter)) {
    m_list->SelectCommand();
  }

  if (IsKeyPressed(keyword::key_backspace)) {
    M_REPLACE_SCENE(scene::SelectScene);
  }
}

void ObjectTestManagerScript::ExecuteLifecycleTest() {
  M_REPLACE_SCENE(scene::ObjectLifeCycle);
}

void ObjectTestManagerScript::ExecuteActivationTest() {
  M_REPLACE_SCENE(scene::ObjectTreeActivation);
}

} /// ::debug::script namespace