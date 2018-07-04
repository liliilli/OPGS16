
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/description.h"

namespace debug::object {

Description::Description() : CText{""} {
  SetFontName("Hangul");
  SetFontSize(16);
  SetColor(opgs16::DColor::White);
  SetOrigin(Origin::DOWN_CENTER);
  SetAlignment(Alignment::CENTER);
  SetWorldPosition({0.f, 48.f, 0.f});
}

} /// ::debug::object namespace