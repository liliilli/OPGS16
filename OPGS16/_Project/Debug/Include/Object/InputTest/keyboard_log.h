#ifndef DEBUG_PROJECT_OBJECT_INPUTTEST_KEYBOARD_LOG_H
#define DEBUG_PROJECT_OBJECT_INPUTTEST_KEYBOARD_LOG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_object.h>

namespace debug::object {

class KeyboardLogObject final : public opgs16::element::UiObject {
public:
  KeyboardLogObject(opgs16::element::canvas::CCanvas* canvas);

private:
  void LocalUpdate() override final;
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_INPUTTEST_KEYBOARD_LOG_H