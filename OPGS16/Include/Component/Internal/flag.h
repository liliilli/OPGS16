#ifndef OPGS16_COMPONENT_INTERNAL_FLAG_H
#define OPGS16_COMPONENT_INTERNAL_FLAG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/flag.h
///
/// @brief
///
///
/// @author Jongmin Yun
/// @log
/// 2018-05-28 Create file.
///

namespace opgs16::component::_internal {

///
/// @enum EStringCacheFlag
/// @brief
///
///
enum class EStringCacheFlag : bool {
  Dirty = false,
  Clean = true
};

} /// ::opgs16::component::_internal namespace.

#endif /// OPGS16_COMPONENT_INTERNAL_FLAG_H