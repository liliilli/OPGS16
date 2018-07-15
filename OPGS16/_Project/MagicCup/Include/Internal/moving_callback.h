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

inline opgs16::DVector3 LinearMove(opgs16::DVector3 src, opgs16::DVector3 dst, float elp) {
  return src * (1 - elp) + dst * elp;
};

inline opgs16::DVector3 BazierMoveLtR(opgs16::DVector3 src, opgs16::DVector3 dst, float elp) {
  const auto via_1 = (src + dst) / 2.f + opgs16::DVector3{0, -48.f, 0};
  const auto left  = src * (1 - elp) + via_1 * elp;
  const auto right = via_1 * (1 - elp) + dst * elp;
  return left * (1 - elp) + right * elp;
}

inline opgs16::DVector3 BazierMoveRtL(opgs16::DVector3 src, opgs16::DVector3 dst, float elp) {
  const auto via_1 = (src + dst) / 2.f + opgs16::DVector3{0, 48.f, 0};
  const auto left  = src * (1 - elp) + via_1 * elp;
  const auto right = via_1 * (1 - elp) + dst * elp;
  return left * (1 - elp) + right * elp;
}

} /// ::magiccup namespace

#endif /// MAGICCUP_INTERNAL_MOVING_CALLBACK_H