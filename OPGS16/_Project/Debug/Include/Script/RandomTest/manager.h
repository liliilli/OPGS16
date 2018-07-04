#ifndef DEBUG_PROJECT_SCRIPT_RANDOMTEST_MANAGER_H
#define DEBUG_PROJECT_SCRIPT_RANDOMTEST_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace debug::object {
class ChoiceList;
}

namespace debug::script {

class RandomTestManager final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(RandomTestManager);

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;

  void InitializeLobbyA();
  void InitializeFloatTestA();

  void InputLobbyA();
  void CleanLobbyAToFloatTestA();

  void ExecuteFloatTest() noexcept;

  bool m_is_pressed = false;

  object::ChoiceList* m_list = nullptr;
  opgs16::element::canvas::CText* m_description = nullptr;

  opgs16::element::canvas::CCanvas* m_obj = nullptr;

  enum class EBigState {
    Lobby,
    FloatTest,
    IntegerTest
  };

  enum class EDetailedState {
    A, B, C, D, E, F
  };

  EBigState m_big_state = EBigState::Lobby;
  EDetailedState m_detailed_state = EDetailedState::A;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_RANDOMTEST_MANAGER_H