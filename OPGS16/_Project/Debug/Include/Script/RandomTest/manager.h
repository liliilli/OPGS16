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

  void ExecuteFloatTest() noexcept;
  void ExecuteFloatTestToLobbyA() noexcept;
  void ExecuteLobbyAToIntegerTestA() noexcept;
  void ExecuteIntegerTestToLobbyA() noexcept;
  void ExecuteLobbyAToPositiveRandomTestA() noexcept;
  void ExecutePositiveRandomTestAToLobbyA() noexcept;
  void ExecuteVector2Test();
  void ExecuteVector3Test();

  void ReturnFromVector2Test();
  void ReturnFromVector3Test();

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;

  void InitializeLobbyA();
  void InitializeFloatTestA();
  void InitializeIntegerTestA();
  void InitializePositiveRandomTestA();
  void InitilaizeVector2UnitTest();
  void InitializeVector3UnitTest();

  void InputLobbyA();

  void CleanLobbyA();
  void CleanFloatTestA();
  void CleanIntegerTestA();
  void CleanPositiveRandomTestA();
  void CleanVector2UnitTest();
  void CleanVector3UnitTest();

  bool m_is_pressed = false;

  opgs16::element::CObject* m_subject = nullptr;
  object::ChoiceList* m_list = nullptr;
  opgs16::element::canvas::CText* m_description = nullptr;

  opgs16::element::canvas::CCanvas* m_obj = nullptr;

  enum class EBigState {
    Lobby,
    FloatTest,
    IntegerTest,
    PositiveRandomTest,
    NegativeRandomTest,
    Vector2Unit,
    Vector3Unit,
  };

  enum class EDetailedState {
    A, B, C, D, E, F
  };

  EBigState m_big_state = EBigState::Lobby;
  EDetailedState m_detailed_state = EDetailedState::A;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_RANDOMTEST_MANAGER_H