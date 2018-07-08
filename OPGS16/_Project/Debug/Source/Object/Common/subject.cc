
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/subject.h"

namespace debug::object {

Subject::Subject() : CText{""} {
  SetFontName("Hangul");
  SetOrigin(IOriginable::Origin::UP_CENTER);
  SetAlignment(IAlignable::Alignment::CENTER);
  SetFontSize(16);
  SetWorldPosition({0.f, -24.f, 0.f});
}

} /// ::debug::object namespace