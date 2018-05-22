
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/logger.h
///
/// @brief 
/// CLogger class implementation file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-06 Add logger implementation file.
/// 2018-04-02 const char* to const wchar_t* for UTF-16 text.
/// 2018-05-22 Refactored deform class structure and recreated to namespace.
/// 2018-05-23 Removed wchar_t parameter version of function.
///

#include <Core\logger.h>  /// Header file

#include <memory>

/// Enhanced assertion
#include <Helper\assert.h>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

namespace {
  
std::shared_ptr<spdlog::logger> m_console = spdlog::stdout_color_mt("OPGS16");

} /// unnamed namespace

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Definition part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

namespace opgs16::debug::log {

void Push(_internal::ELogMessageType msg_type, const char* log_message) {
  using _internal::ELogMessageType;

  switch (msg_type) {
  case ELogMessageType::Info: 
    m_console->info(log_message);
    break;
  case ELogMessageType::Debug: 
    m_console->debug(log_message);
    break;
  case ELogMessageType::Warning:
    m_console->warn(log_message);
    break;
  case ELogMessageType::Critical:
    m_console->critical(log_message);
    break;
  case ELogMessageType::Error: 
    m_console->error(log_message);
    break;
  }
}

} /// ::opgs16::debug::log

namespace opgs16::debug::log::____ {

std::weak_ptr<spdlog::logger> Get() {
  return m_console;
}

} /// ::opgs16::debug::log::____
