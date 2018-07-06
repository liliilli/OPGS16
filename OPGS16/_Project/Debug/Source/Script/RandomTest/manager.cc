
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/manager.h"

#include <Element/Canvas/canvas.h>
#include <Element/Canvas/text.h>
#include <Helper/Type/vectori2.h>

#include <manager/input_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/copyright.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/Common/gotoback.h"
#include "../../../Include/Object/Common/simplelog.h"

#include "../../../Include/Object/SceneSelect/choice_list.h"
#include "../../../Include/Script/RandomTest/float.h"
#include "../../../Include/Script/RandomTest/integer.h"

namespace {

std::vector<std::pair<std::string, std::string>>
command_list = {
  {"Float random test", u8"단실수 랜덤 테스트를 합니다."},
  {"Integer random test", u8"32bit 정수 랜덤 테스트를 합니다."},
  {"Positive random test", ""},
  {"Negative random test", ""},
  {"DVector2 random test", ""},
  {"DVector3 random test", ""},
  {"DVector2 range dist test", ""},
  {"DVector3 range dist test", ""}
};

} /// unnamed namespace

namespace debug::script {

void RandomTestManager::Initiate() {
  m_obj = static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject());
  m_obj->Instantiate<object::GotoBackSign>("Back");
  m_obj->Instantiate<object::Copyright>("Copyright");

  InitializeLobbyA();
}

void RandomTestManager::Start() { }

void RandomTestManager::Update(float delta_time) {
  switch (m_big_state) {
  case EBigState::Lobby:
    switch (m_detailed_state) {
    case EDetailedState::A:
      InputLobbyA();
      break;
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    }
    break;
  case EBigState::FloatTest: break;
  case EBigState::IntegerTest: break;
  }
}

void RandomTestManager::InitializeLobbyA() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto subject = m_obj->Instantiate<CText>("Subject", "Random feature test");
  subject->SetFontName("Hangul");
  subject->SetOrigin(IOriginable::Origin::UP_CENTER);
  subject->SetAlignment(IAlignable::Alignment::CENTER);
  subject->SetFontSize(16);
  subject->SetWorldPosition({0.f, -24.f, 0.f});

  m_list = m_obj->Instantiate<ChoiceList>(
      "CommandList", "opSystem", m_obj,
      std::vector<std::string>{
          command_list[0].first, command_list[1].first,
          command_list[2].first, command_list[3].first,
          command_list[4].first, command_list[5].first,
          command_list[6].first, command_list[7].first
      }
  );
  m_list->SetItemSize(12);
  m_list->SetFontSize(8);
  m_list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  m_list->SetTextureIndex(8);
  m_list->SetSelectedColor(opgs16::DColor::Aqua);
  m_list->SetNormalColor(opgs16::DColor::Gray);
  m_list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  m_list->SetWorldPosition({32.f, 96.f, 0.f});

  m_list->SetFunction(0, std::bind(&RandomTestManager::ExecuteFloatTest, this));
  m_list->SetFunction(1, std::bind(&RandomTestManager::ExecuteLobbyAToIntegerTestA, this));

  m_description = m_obj->Instantiate<object::Description>("Desc");
  m_description->SetText(command_list[m_list->GetCursorIndex()].second);
}

void RandomTestManager::InputLobbyA() {
  using opgs16::manager::input::GetKeyValue;
  using opgs16::manager::input::IsKeyPressed;
  using opgs16::manager::input::IsKeyReleased;

  if (m_is_pressed == true) {
    if (IsKeyReleased("Vertical")) m_is_pressed = false;
    return;
  }

  const auto key_val = GetKeyValue("Vertical");
  if (key_val == 1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Up);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(command_list[new_idx].second);
    }
    m_is_pressed = true;
  }
  else if (key_val == -1.0f) {
    const auto idx = m_list->GetCursorIndex();
    m_list->MoveCursor(object::EDirection::Down);

    if (const auto new_idx = m_list->GetCursorIndex(); new_idx != idx) {
      m_description->SetText(command_list[new_idx].second);
    }
    m_is_pressed = true;
  }

  if (IsKeyPressed("Horizontal")) {
    m_list->SelectCommand();
  }
}

void RandomTestManager::InitializeFloatTestA() {
  m_obj->AddComponent<script::FloatTest>(*m_obj);
}

void RandomTestManager::InitializeIntegerTestA() {
  m_obj->AddComponent<script::IntegerTest>(*m_obj);
}

void RandomTestManager::CleanLobbyA() {
  if (!m_obj) {
    PUSH_LOG_CRITICAL("Binded object address is nullptr.");
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  m_obj->DestroyChild("Subject");
  m_obj->DestroyChild("CommandList");
  m_obj->DestroyChild("Desc");
}

void RandomTestManager::CleanFloatTestA() {
  if (!m_obj->RemoveComponent<script::FloatTest>()) {
    PHITOS_UNEXPECTED_BRANCH();
  }
}

void RandomTestManager::CleanIntegerTestA() {
  if (!m_obj->RemoveComponent<script::IntegerTest>()) {
    PHITOS_UNEXPECTED_BRANCH();
  }
}

void RandomTestManager::ExecuteFloatTest() noexcept {
  PUSH_LOG_INFO("ExecuteFloatTest()");

  CleanLobbyA();
  m_big_state = EBigState::FloatTest;
  m_detailed_state = EDetailedState::A;
  InitializeFloatTestA();
}

void RandomTestManager::ExecuteLobbyAToIntegerTestA() noexcept {
  PUSH_LOG_INFO("ExecuteLobbyAToIntegerTestA()");

  CleanLobbyA();
  m_big_state = EBigState::IntegerTest;
  m_detailed_state = EDetailedState::A;
  InitializeIntegerTestA();
}

void RandomTestManager::ExecuteFloatTestToLobbyA() noexcept {
  PUSH_LOG_INFO("ExecuteFloatTestToLobbyA()");

  CleanFloatTestA();
  m_big_state = EBigState::Lobby;
  m_detailed_state = EDetailedState::A;
  InitializeLobbyA();
}

void RandomTestManager::ExecuteIntegerTestToLobbyA() noexcept {
  PUSH_LOG_INFO("ExecuteFloatTestToLobbyA()");

  CleanIntegerTestA();
  m_big_state = EBigState::Lobby;
  m_detailed_state = EDetailedState::A;
  InitializeLobbyA();
}


} /// ::debug::script namespace