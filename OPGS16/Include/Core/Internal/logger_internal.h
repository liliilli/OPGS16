#ifndef OPGS16_CORE_INTERNAL_LOGGER_H
#define OPGS16_CORE_INTERNAL_LOGGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/Internal/logger_internal.h
///
/// @brief 
/// Internal Type used to in ::opgs16::debug::log namespace.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-06 Create file.
/// 2018-05-22 Refactoring and Add Debug, Critical.
///

namespace opgs16::debug::_internal {

///
/// @enum ELogMessageType
///
/// @brief
/// Represents logger message type.
///
enum class ELogMessageType {
  Info,
  Debug,
  Warning,  
  Critical,
  Error
};

} /// ::opgs16::debug::_internal

#endif /// OPGS16_CORE_INTERNAL_LOGGER_H

