#ifndef DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_COMPACT_H
#define DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_COMPACT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>

namespace debug::script {

class KeyboardCompactTest final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(KeyboardCompactTest);

private:
  void Initiate() override final;
  void Start() override final {};
  void Update(float delta_time) override final {};

};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_INPUTTEST_KEYBOARD_COMPACT_H
