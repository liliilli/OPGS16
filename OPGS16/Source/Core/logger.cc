#include <precompiled.h>
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

/// Header file
#include <Core/logger.h>

#include <memory>
#include <vector>

#include <Core/core_setting.h>

//!
//! Member container
//!

namespace {

std::vector<spdlog::sink_ptr>   s_log_sinks;
std::shared_ptr<spdlog::logger> s_logger;

} /// unnamed namespace

//!
//! Definition part
//!

namespace opgs16::debug::log {

void Push(_internal::ELogMessageType msg_type, const char* log_message) {
  using _internal::ELogMessageType;
  if (!s_logger) return;

  switch (msg_type) {
  case ELogMessageType::Info:
    s_logger->info(log_message);
    break;
  case ELogMessageType::Debug:
    s_logger->debug(log_message);
    break;
  case ELogMessageType::Warning:
    s_logger->warn(log_message);
    break;
  case ELogMessageType::Critical:
    s_logger->critical(log_message);
    break;
  case ELogMessageType::Error:
    s_logger->error(log_message);
    break;
  }
}

} /// ::opgs16::debug::log

namespace opgs16::debug::log::____ {

void InitiateLogger() {
  if (!setting::IsEnableLoggingFeature())
    return;

  if (setting::IsEnableLoggingConsole()) {
#ifdef _WIN32
    s_log_sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
    s_log_sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif
  }

  if (setting::IsEnableLoggingToFile()) {
    // Caution :: To let spdlog create log file to specific path,
    // directories must be created in advance.
    s_log_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("./logs/debug_log.txt", false));
  }

  s_logger = std::make_shared<spdlog::logger>("opgs16", s_log_sinks.begin(), s_log_sinks.end());
  s_logger->set_level(spdlog::level::trace);

  spdlog::register_logger(s_logger);
}

std::weak_ptr<spdlog::logger> Get() {
  return s_logger;
}

} /// ::opgs16::debug::log::____
