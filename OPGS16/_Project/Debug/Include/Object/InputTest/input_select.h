#ifndef DEBUG_PROJECT_OBJECT_INPUTTEST_INPUT_SELECT_H
#define DEBUG_PROJECT_OBJECT_INPUTTEST_INPUT_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/canvas.h>

namespace debug::object {

class InputSelectObject final : public opgs16::element::canvas::CCanvas {
public:
  InputSelectObject();
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_INPUTTEST_INPUT_SELECT_H