#ifndef OPGS16_CORE_LOGGER_H
#define OPGS16_CORE_LOGGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core\logger.h
///
/// @brief 
/// CLogger class declaration file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-06 Add logger frame.
/// 2018-03-11 Attach spdlog logging library.
/// 2018-04-02 const char* to const wchar_t* for multibyte text.
/// 2018-05-22 Refactored deform class structure and recreated to namespace.
/// 2018-05-23 Removed wchar_t parameter version of function.
///

#include <Core\Internal\logger_internal.h>  /// ::opgs16::debug::_internal
#include <memory>

/// Third party logger library
#include <spdlog\spdlog.h>

///
/// @namespace opgs16::debug::log
///
/// @brief 
/// This namespace implements logger components.
///
namespace opgs16::debug::log {

///
/// @brief
/// Push message into logger
///
/// @param[in] msg_type Message type for logging. 
/// @param[in] log_message Log message to output on console or log file.
///
void Push(_internal::ELogMessageType msg_type, const char* log_message);

} /// ::opgs16::debug::log

///
/// @namespace opgs16::debug::log::____
///
/// @brief
/// Private namespace of opgs16::debug::log namespace.
///
namespace opgs16::debug::log::____ {

///
/// @brief
/// Get reference of logger instance inside.
///
/// @return Reference of logger instance.
///
std::weak_ptr<spdlog::logger> Get();

} /// ::opgs16::debug::log::____


#endif /// OPGS16_CORE_LOGGER_H
