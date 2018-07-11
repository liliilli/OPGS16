
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/InputTest/select_lobby.h"

#include <vector>

#include <Element/Canvas/canvas.h>
#include <Manager/input_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Internal/keyword.h"
#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/copyright.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/Common/gotoback.h"
#include "../../../Include/Object/Common/subject.h"
#include "../../../Include/Script/InputTest/keyboard_compact.h"

namespace {

std::vector<std::string> command_list = {
  "Keyboard Compact Test",
  "Keyboard Binding Key Test"
  "Mouse Compact Test",
  "Mouse Raytracing Test"
};

std::vector<std::string> description_list = {
  u8"키보드 전체 키 입력\n테스트를 합니다.",
  u8"바인딩 된 키보드 입력\n테스트를 합니다."
  u8"마우스 키 입력\n테스트를 합니다.",
  u8"마우스 입력 레이트레이싱\n테스트를 합니다."
};

} /// unnamed namespace

namespace debug::script {

void InputTestLobbyScript::Initiate() {
  m_bind_obj = static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject());
  m_bind_obj->Instantiate<object::GotoBackSign>("Back");
  m_bind_obj->Instantiate<object::Copyright>("Copyright");

  InitializeLobby();
}

void InputTestLobbyScript::Update(float delta_time) {
  switch (m_state) {
  case EState::Lobby:
    InputLobby();
    break;
  default: break;
  }
}

void InputTestLobbyScript::InitializeLobby() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto subject = m_bind_obj->Instantiate<object::Subject>("Subject");
  subject->SetText("Random feature test");
  m_subject = subject;

  auto list = m_bind_obj->Instantiate<ChoiceList>(
      "Command", "opSystem",
      static_cast<opgs16::element::canvas::CCanvas*>(m_bind_obj), command_list);
  list->SetItemSize(12);
  list->SetFontSize(8);
  list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  list->SetTextureIndex(8);
  list->SetSelectedColor(opgs16::DColor::Aqua);
  list->SetNormalColor(opgs16::DColor::Gray);
  list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  list->SetWorldPosition({32.f, 96.f, 0.f});
  list->SetFunction(0, std::bind(&InputTestLobbyScript::ExecuteKeyboardCompactTest, this));
  m_choice_list = list;

  auto description = m_bind_obj->Instantiate<object::Description>("Desc");
  description->SetText(description_list[list->GetCursorIndex()]);
  m_description = description;
}

void InputTestLobbyScript::InputLobby() {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;

  if (m_is_pressed == true) {
    if (IsKeyReleased(keyword::key_axis_vert))
      m_is_pressed = false;
    return;
  }

  const auto key_val = GetKeyValue(keyword::key_axis_vert);
  if (key_val == 1.0f) {
    const auto idx = m_choice_list->GetCursorIndex();
    m_choice_list->MoveCursor(object::EDirection::Up);

    if (const auto new_idx = m_choice_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_list[new_idx]);
    }
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    const auto idx = m_choice_list->GetCursorIndex();
    m_choice_list->MoveCursor(object::EDirection::Down);

    if (const auto new_idx = m_choice_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(description_list[new_idx]);
    }
    m_is_pressed = true;
  }

  if (IsKeyPressed(keyword::key_axis_horz)) {
    m_choice_list->SelectCommand();
  }
}

void InputTestLobbyScript::InitializeKeyboardCompact() {
  m_bind_obj->AddComponent<script::KeyboardCompactTest>(*m_bind_obj);
}

void InputTestLobbyScript::CleanLobby() {
  if (!m_bind_obj) {
    PUSH_LOG_CRITICAL("Binded object address is nullptr.");
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  m_bind_obj->DestroyChild(*m_subject);
  m_bind_obj->DestroyChild(*m_choice_list);
  m_bind_obj->DestroyChild(*m_description);

  m_subject = nullptr;
  m_choice_list = nullptr;
  m_description = nullptr;
}

void InputTestLobbyScript::CleanKeyboardCompact() {
  if (!m_bind_obj->RemoveComponent<script::KeyboardCompactTest>()) {
    PHITOS_UNEXPECTED_BRANCH();
  }
}

void InputTestLobbyScript::ExecuteKeyboardCompactTest() {
  PUSH_LOG_INFO("ExecuteKeyboardCompactTest()");

  CleanLobby();
  m_state = EState::KeyboardTest;
  InitializeKeyboardCompact();
}

void InputTestLobbyScript::ReturnFromKeyboardCompactTest() {
  PUSH_LOG_INFO("ReturnFromKeyboardCompactTest()");

  CleanKeyboardCompact();
  m_state = EState::Lobby;
  InitializeLobby();
}

} /// ::debug::script namespace