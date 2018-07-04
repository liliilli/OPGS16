
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/Common/copyright.h"

namespace debug::object {

Copyright::Copyright() : CText{"2018 Jongmin Yun(Neu.)"} {
  SetFontName("opSystem");
  SetFontSize(8);
  SetColor(opgs16::DColor::Gray);
  SetOrigin(IOriginable::Origin::DOWN_RIGHT);
  SetAlignment(IAlignable::Alignment::RIGHT);
  SetWorldPosition({-8.f, 8.f, 0.f});
}

} /// ::debug::object namespace