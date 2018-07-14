#ifndef MAGICCUP_INTERNAL_MOVING_CALLBACK_H
#define MAGICCUP_INTERNAL_MOVING_CALLBACK_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Helper/Type/vector3.h>

namespace magiccup {

opgs16::DVector3 DontMove(opgs16::DVector3 src, opgs16::DVector3 dst, float elp) {
  return src * (1 - elp) + dst * elp;
};

} /// ::magiccup namespace

#endif /// MAGICCUP_INTERNAL_MOVING_CALLBACK_H