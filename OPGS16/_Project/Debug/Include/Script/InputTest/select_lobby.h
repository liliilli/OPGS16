#ifndef DEBUG_PROJECT_SCRIPT_INPUTTEST_SELECT_LOBBY_H
#define DEBUG_PROJECT_SCRIPT_INPUTTEST_SELECT_LOBBY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <vector>

#include <Component/script_frame.h>

namespace debug::object {
class ChoiceList;
class Subject;
}

namespace debug::script {

class InputTestLobbyScript final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(InputTestLobbyScript);

  void ExecuteKeyboardCompactTest();

  void ReturnFromKeyboardCompactTest();

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;
  void Destroy() override {};

  void InitializeLobby();
  void InitializeKeyboardCompact();

  void InputLobby();

  void CleanLobby();
  void CleanKeyboardCompact();

  enum class EState {
    Lobby, KeyboardTest, KeyboardBinding, MouseTest, MouseRaytrace,
  };
  EState m_state = EState::Lobby;

  opgs16::element::CObject* m_bind_obj = nullptr;

  object::ChoiceList* m_choice_list = nullptr;
  object::Subject* m_subject = nullptr;
  opgs16::element::canvas::CText* m_description = nullptr;

  bool m_is_pressed = false;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_INPUTTEST_SELECT_LOBBY_H