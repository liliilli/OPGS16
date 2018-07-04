
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../Include/Object/Common/gotoback.h"

namespace debug::object {

GotoBackSign::GotoBackSign() : CText{"Push Backspace to go back."} {
  SetFontName("opSystem");
  SetFontSize(8);
  SetColor(opgs16::DColor::Gray);
  SetOrigin(Origin::DOWN_RIGHT);
  SetAlignment(Alignment::RIGHT);
  SetWorldPosition({-8.f, 16.f, 0.f});
}

} /// ::debug::object namespace