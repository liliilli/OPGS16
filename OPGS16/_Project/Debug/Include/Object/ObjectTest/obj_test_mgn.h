#ifndef DEBUG_PROJECT_OBJECT_OBJECTTEST_OBJECT_SELECT_H
#define DEBUG_PROJECT_OBJECT_OBJECTTEST_OBJECT_SELECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/canvas.h>

namespace debug::object {

class ObjectTestManagerCanvas final : public opgs16::element::canvas::CCanvas {
public:
  ObjectTestManagerCanvas();
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_OBJECT_OBJECTTEST_OBJECT_SELECT_H