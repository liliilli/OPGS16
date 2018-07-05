
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/color.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-06 Set up static const built-in colors.
///

/// Header file
#include <Helper/Type/color.h>
#include <Helper/Type/color32.h>

namespace opgs16 {

const DColor DColor::Aqua = DColor32{0x00, 0xAA, 0xFF};
const DColor DColor::Black = DColor32{0x00, 0x00, 0x00};
const DColor DColor::Blue = DColor32{0x00, 0x00, 0xFF};
const DColor DColor::DarkRed = DColor32{0xAA, 0x00, 0x00};
const DColor DColor::DarkGray = DColor32{0x55, 0x55, 0x55};
const DColor DColor::DarkGreen = DColor32{0x00, 0xAA, 0x00};
const DColor DColor::Gold = DColor32{0xFF, 0xAA, 0x00};
const DColor DColor::Gray = DColor32{0xAA, 0xAA, 0xAA};
const DColor DColor::Green = DColor32{0x00, 0xFF, 0x00};
const DColor DColor::Magenta = DColor32{0xAA, 0x00, 0xAA};
const DColor DColor::Orange = DColor32{0xFF, 0x55, 0x00};
const DColor DColor::Purple = DColor32{0xAA, 0x00, 0xFF};
const DColor DColor::Red = DColor32{0xFF, 0x00, 0x00};
const DColor DColor::White = DColor32{0xFF, 0xFF, 0xFF};
const DColor DColor::Yellow = DColor32{0xFF, 0xFF, 0x00};

} /// ::opgs16 namespace
