#ifndef DEBUG_PROJECT_COMMON_GOTOBACK_H
#define DEBUG_PROJECT_COMMON_GOTOBACK_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/text.h>

namespace debug::object {

class GotoBackSign final : public opgs16::element::canvas::CText {
public:
  GotoBackSign();
};

} /// ::debug::object

#endif /// DEBUG_PROJECT_COMMON_GOTOBACK_H