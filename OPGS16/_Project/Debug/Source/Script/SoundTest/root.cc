
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/SoundTest/root.h"

#include <vector>

#include <Element/Canvas/canvas.h>

#include <Manager/input_manager.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Internal/keyword.h"

#include "../../../Include/Object/Common/choice_list.h"
#include "../../../Include/Object/Common/copyright.h"
#include "../../../Include/Object/Common/description.h"
#include "../../../Include/Object/Common/gotoback.h"
#include "../../../Include/Object/Common/subject.h"

#include "../../../Include/Scene/SoundTest/sound.h"

namespace {

std::vector<std::pair<std::string, std::string>>
command_list = {
  {"BGM Test", u8"BGM 테스트를 실시합니다."},
  {"Effect Test", u8"Effect 사운드 테스트를\n실시합니다."},
  {"Surround Test", u8"거리에 따라 달리 들리는\n음향 테스트를 합니다."}
};

} /// unnamed namespace

namespace debug::script {

void SoundTestRoot::Initiate() {
  m_obj = static_cast<opgs16::element::canvas::CCanvas*>(&GetBindObject());
  m_obj->Instantiate<object::GotoBackSign>("Back");
  m_obj->Instantiate<object::Copyright>("Copyright");

  InitializeLobby();
}

void SoundTestRoot::Start() { }

void SoundTestRoot::Update(float delta_time) {
  switch (m_big_state){
  case EBigState::Lobby:
    InputLobby();
    break;
  default: break;
  }
}

void SoundTestRoot::InitializeLobby() {
  using opgs16::element::canvas::CText;
  using object::ChoiceList;

  auto subject = m_obj->Instantiate<object::Subject>("Subject");
  subject->SetText("Sound feature test");
  m_subject = subject;

  m_list = m_obj->Instantiate<ChoiceList>("CommandList", "opSystem", m_obj,
      std::vector<std::string>{
          command_list[0].first, command_list[1].first, command_list[2].first
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

  m_list->SetFunction(0, std::bind(&SoundTestRoot::ExecuteBgmTest, this));
  m_list->SetFunction(1, std::bind(&SoundTestRoot::ExecuteEffectTest, this));
  m_list->SetFunction(2, std::bind(&SoundTestRoot::ExecuteSurroundTest, this));

  m_description = m_obj->Instantiate<object::Description>("Desc");
  m_description->SetText(command_list[m_list->GetCursorIndex()].second);
}

void SoundTestRoot::InputLobby() {
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

  if (IsKeyPressed(keyword::key_axis_horz)) {
    m_list->SelectCommand();
  }
}

void SoundTestRoot::ExecuteLobby() {

}

void SoundTestRoot::ExecuteBgmTest() {
  M_PUSH_SCENE(scene::SoundTestScnee, false);
}

void SoundTestRoot::ExecuteEffectTest() {

}

void SoundTestRoot::ExecuteEntireTest() {

}

void SoundTestRoot::ExecuteSurroundTest() {

}

} /// ::debug::script namespace