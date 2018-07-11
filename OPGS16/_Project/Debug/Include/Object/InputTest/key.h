#ifndef DEBUG_PROJECT_OBJECT_INPUTTEST_KEY_H
#define DEBUG_PROJECT_OBJECT_INPUTTEST_KEY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/ui_object.h>

#include "../../../Include/Internal/key_code.h"

namespace debug::object {

class KeyImage final : public opgs16::element::UiObject {
public:
  KeyImage(keyword::EKeyboardKeyCode key_code,
           const opgs16::element::canvas::CCanvas* canvas_ref);

  void LocalUpdate() override final;
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_INPUTTEST_KEY_H