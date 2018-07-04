#ifndef DEBUG_PROJECT_DESCRIPTION_H
#define DEBUG_PROJECT_DESCRIPTION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/text.h>

namespace debug::object {

class Description final : public opgs16::element::canvas::CText {
public:
  Description();
};

} /// ::debug::object namespace

#endif /// DEBUG_PROJECT_DESCRIPTION_H