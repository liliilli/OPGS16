
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RenderingTest/select_menu.h"

#include <Element/Canvas/canvas.h>
#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/copyright.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/Common/gotoback.h"
#include "../../../Include/Object/Common/subject.h"

#include "../../../Include/Script/RenderingTest/procedural2d.h"

#include "../../../Include/Internal/keyword.h"

namespace {

std::vector<std::string> command_list = {
  "Procedural2DRendering test"
};

std::vector<std::string> description_list = {
  u8"절차적 텍스쳐의\n묘화 테스트를 합니다."
};

} /// unnamed namespace

namespace debug::script {

void RenderingTestSelectMenu::Initiate() {
  m_bind_object = static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject());
  m_bind_object->Instantiate<object::GotoBackSign>("Back");
  m_bind_object->Instantiate<object::Copyright>("Copyright");

  InitializeLobby();
}

void RenderingTestSelectMenu::Update(float delta_time) {
  switch (m_state) {
  case EState::Lobby:
    InputLobby();
    break;
  case EState::Procedural2DRenderingTest:
    break;
  }
}

void RenderingTestSelectMenu::ExecuteProcedural2DRenderingTest() {
  PUSH_LOG_INFO("InitializeProcedural2DRenderingTest()");

  CleanLobby();
  m_state = EState::Procedural2DRenderingTest;
  InitializeProcedural2DRenderingTest();
}

void RenderingTestSelectMenu::ReturnFromProcedural2DRendering() {
  PUSH_LOG_INFO("ReturnFromProcedural2DRendering()");

  CleanProcedural2DRenderingTest();
  m_state = EState::Lobby;
  InitializeLobby();
}

void RenderingTestSelectMenu::InitializeLobby() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto subject = m_bind_object->Instantiate<object::Subject>("Subject");
  subject->SetText("Random feature test");
  m_subject = subject;

  m_list = m_bind_object->Instantiate<ChoiceList>(
      "CommandList", "opSystem", m_bind_object, command_list);
  m_list->SetItemSize(12);
  m_list->SetFontSize(8);
  m_list->SetCursorSize(opgs16::DVectorInt2{16, 16});
  m_list->SetTextureIndex(8);
  m_list->SetSelectedColor(opgs16::DColor::Aqua);
  m_list->SetNormalColor(opgs16::DColor::Gray);
  m_list->SetOrigin(IOriginable::Origin::DOWN_LEFT);
  m_list->SetWorldPosition({32.f, 96.f, 0.f});

  m_list->SetFunction(0,
      std::bind(&RenderingTestSelectMenu::ExecuteProcedural2DRenderingTest, this));

  m_description = m_bind_object->Instantiate<object::Description>("Desc");
  m_description->SetText(description_list[m_list->GetCursorIndex()]);
}

void RenderingTestSelectMenu::InitializeProcedural2DRenderingTest() {
  m_bind_object->AddComponent<script::Procedural2DRenderingTest>(*m_bind_object);
}

void RenderingTestSelectMenu::InputLobby() {
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

  if (IsKeyPressed(keyword::key_axis_horz)) {
    m_list->SelectCommand();
  }
}

void RenderingTestSelectMenu::CleanLobby() {
  if (!m_bind_object) {
    PUSH_LOG_CRITICAL("Binded object address is nullptr.");
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  m_bind_object->DestroyChild(*m_subject);
  m_bind_object->DestroyChild(*m_list);
  m_bind_object->DestroyChild(*m_description);

  m_subject = nullptr;
  m_list = nullptr;
  m_description = nullptr;
}

void RenderingTestSelectMenu::CleanProcedural2DRenderingTest() {
  if (!m_bind_object->RemoveComponent<script::Procedural2DRenderingTest>()) {
    PHITOS_UNEXPECTED_BRANCH();
  }
}

} /// ::debug::script namespace