#ifndef OPGS16_MANAGER_INTERNAL_FLAG_H
#define OPGS16_MANAGER_INTERNAL_FLAG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/flag.h
///
/// @brief
/// Strong boolean enum aggregation file for runtime check of ::opgs16::manager
/// This file must not be together with the other flag.h file.
/// namespace is duplicated, might occur unexpcted behavior or error. 
///
/// @log
/// 2018-05-20 Create file and make enum class for boolean flag.
///

namespace opgs16::debug {

///
/// @enum EInitiated
/// @brief
/// This enum class is for checking and preventing duplicated 
/// ::opgs16::manager::(namespace)::Initiate() call from somewhere.
///
enum class EInitiated : bool {
  NotInitiated = false,
  Initiated = true
};

} /// ::opgs16::debug

#endif /// MANAGER_INTERNAL_MANAGER_FLAG_H