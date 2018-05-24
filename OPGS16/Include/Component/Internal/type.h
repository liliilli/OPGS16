#ifndef OPGS16_COMPONENT_INTERNAL_TYPE_H
#define OPGS16_COMPONENT_INTERNAL_TYPE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/component_.h
///
/// @brief
/// Manages component internal type for sorting and static conversion.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-25 Create file.
///

namespace opgs16::component::_internal {

///
/// @enum EComponentType
/// @brief
///
///
enum class EComponentType {
  Normal,
  Script
};

} /// ::opgs16::component::_internal

#endif /// OPGS16_COMPONENT_INTERNAL_TYPE_H