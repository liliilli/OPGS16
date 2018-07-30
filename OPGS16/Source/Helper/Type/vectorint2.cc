#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/vectorint2.cc
///
/// @brief
/// Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-31 Create file.
///

/// Header file
#include <Helper/Type/vectori2.h>
#include <Helper/Math/math.h>

namespace opgs16 {

bool DVectorInt2::IsAllZero(const opgs16::DVectorInt2& vector) noexcept {
  return opgs16::math::IsAllZero(vector);
}

bool DVectorInt2::IsAllZero() const noexcept {
  return opgs16::math::IsAllZero(*this);
}

} /// ::opgs16 namespace