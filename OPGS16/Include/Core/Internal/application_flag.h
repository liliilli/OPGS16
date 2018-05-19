#ifndef OPGS16_CORE_INTERNAL_APPLICATION_FLAG_H
#define OPGS16_CORE_INTERNAL_APPLICATION_FLAG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/Internal/application_flag.h
///
/// @brief
/// Strong boolean enum aggregation file for runtime check of ::opgs16::entry
///
/// @log
/// 2018-05-20 Create file and make enum class for boolean flag.
///

namespace opgs16::entry::_internal {

///
/// @enum EInitiated
/// @brief
/// This enum class is for checking and preventing duplicated 
/// ::opgs16::entry::Initiate() call from somewhere.
///
enum class EInitiated : bool {
  NotInitiated = false,
  Initiated = true
};

///
/// @enum EOperated
/// @brief
/// This enum class is for checking and preventing duplicated
/// ::opgs16::entry::Run() call from somewhere in the project.
///
enum class EOperated : bool {
  NotOperated = false,
  Operated = true
};

} /// ::opgs16::entry::_internal

#endif /// OPGS16_CORE_INTERNAL_APPLICATION_FLAG_H

