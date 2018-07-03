#ifndef DEBUG_PROJECT_OBJECT_RANDOMTEST_MANAGER_H
#define DEBUG_PROJECT_OBJECT_RANDOMTEST_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// ::opgs16::element::CObject
#include <Element/Canvas/canvas.h>

namespace debug::object {

class RandomManager final : public opgs16::element::canvas::CCanvas {
public:
  RandomManager();
};

} /// ::debug::object

#endif /// DEBUG_PROJECT_OBJECT_RANDOMTEST_MANAGER_H