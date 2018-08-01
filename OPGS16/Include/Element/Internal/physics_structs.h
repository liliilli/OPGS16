#ifndef OPGS16_ELEMENT_INTERNAL_PHYSICS_STRUCT_H
#define OPGS16_ELEMENT_INTERNAL_PHYSICS_STRUCT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/physics_structs.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

namespace opgs16::component::_internal {

struct alignas(4) DLinearLimitFactor {
  bool x = false;
  bool y = false;
  bool z = false;
};

} /// ::opgs16::component::_internal namespace

#endif ///