#ifndef DEBUG_PROJECT_SCRIPT_OBJECTTEST_OBJ_TEST_MGN_SCRIPT_H
#define DEBUG_PROJECT_SCRIPT_OBJECTTEST_OBJ_TEST_MGN_SCRIPT_H

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

class ObjectTestManagerScript final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(ObjectTestManagerScript);

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final;

  void ExecuteLifecycleTest();
  void ExecuteActivationTest();

  opgs16::element::CObject* m_subject = nullptr;
  object::ChoiceList*       m_list = nullptr;
  opgs16::element::canvas::CText*   m_description = nullptr;
  opgs16::element::canvas::CCanvas* m_obj = nullptr;

  bool m_is_pressed = false;

};

} /// ::debug::script

#endif /// DEBUG_PROJECT_SCRIPT_OBJECTTEST_OBJ_TEST_MGN_SCRIPT_H