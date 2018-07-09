#ifndef DEBUG_PROJECT_SCRIPT_RENDERINGTEST_SELECT_MENU_H
#define DEBUG_PROJECT_SCRIPT_RENDERINGTEST_SELECT_MENU_H

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

class RenderingTestSelectMenu final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(RenderingTestSelectMenu);

  void ExecuteProcedural2DRenderingTest();
  void ReturnFromProcedural2DRendering();

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;

  void InitializeLobby();
  void InitializeProcedural2DRenderingTest();

  void InputLobby();

  void CleanLobby();
  void CleanProcedural2DRenderingTest();

  bool m_is_pressed = false;

  opgs16::element::CObject* m_subject = nullptr;
  opgs16::element::canvas::CText* m_description = nullptr;
  opgs16::element::canvas::CCanvas* m_bind_object = nullptr;
  object::ChoiceList* m_list = nullptr;

  enum class EState {
    Lobby,
    Procedural2DRenderingTest
  };

  EState m_state = EState::Lobby;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_RENDERINGTEST_SELECT_MENU_H