#ifndef DEBUG_PROEJECT_SCRIPT_SCENESELECT_MANAGEMENT_H
#define DEBUG_PROEJECT_SCRIPT_SCENESELECT_MANAGEMENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

//!
//! Forward declaration
//!
namespace debug::object { class ChoiceList; }

//!
//! Implementation
//!

namespace debug::script {

class Management final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(Management);

private:
  void Initiate() override final;
  void Start() override final;
  void Update(float delta_time) override final;

  void Input();

  void ExecuteLifecycleTest();
  void ExecuteRandomFeatureTest();
  void ExecuteSoundTest();
  void ExecuteInputTest();
  void ExecuteRenderingTest();

  object::ChoiceList* m_list = nullptr;
  opgs16::element::canvas::CText* m_description = nullptr;
  bool m_is_pressed = false;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROEJECT_SCRIPT_SCENESELECT_MANAGEMENT_H