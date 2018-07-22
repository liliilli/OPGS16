#ifndef OPGS16_HEADERS_IMPORT_LOGGER_H
#define OPGS16_HEADERS_IMPORT_LOGGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Headers/import_logger.h
///
/// @brief Logger importer.
///
/// @author Jongmin Yun
/// @log
/// 2018-04-06 Create file.
/// 2018-05-22
/// Add variant parameter version of logging macroes,
/// and Add DEBUG and CRITICAL priority log mode.
///

/// Enhanced assertion
#include <Phitos/Dbg/assert.h>

#if defined(_DEBUG)

#include <Core\logger.h>

#define PUSH_LOG_INFO(__MAString__) \
  ::opgs16::debug::log::Push(opgs16::debug::_internal::ELogMessageType::Info, \
                             __MAString__)

#define PUSH_LOG_DEBUG(__MAString__) \
  ::opgs16::debug::log::Push(opgs16::debug::_internal::ELogMessageType::Debug, \
                             __MAString__)

#define PUSH_LOG_WARN(__MAString__) \
  ::opgs16::debug::log::Push(opgs16::debug::_internal::ELogMessageType::Warning, \
                             __MAString__)

#define PUSH_LOG_CRITICAL(__MAString__) \
  ::opgs16::debug::log::Push(opgs16::debug::_internal::ELogMessageType::Critical, \
                             __MAString__)

#define PUSH_LOG_ERRO(__MAString__) \
  ::opgs16::debug::log::Push(opgs16::debug::_internal::ELogMessageType::Error, \
                             __MAString__)

#define PUSH_LOG_INFO_EXT(__MAString__, ...) \
  if (auto spt = ::opgs16::debug::log::____::Get().lock()) { \
    spt->info(__MAString__, __VA_ARGS__); \
  } else { \
    PHITOS_NOT_IMPLEMENTED_ASSERT(); \
  }

///
/// @todo Change info to debug.
///

#define PUSH_LOG_DEBUG_EXT(__MAString__, ...) \
  if (auto spt = ::opgs16::debug::log::____::Get().lock()) { \
    spt->info(__MAString__, __VA_ARGS__); \
  } else { \
    PHITOS_NOT_IMPLEMENTED_ASSERT(); \
  }

#define PUSH_LOG_WARN_EXT(__MAString__, ...) \
  if (auto spt = ::opgs16::debug::log::____::Get().lock()) { \
    spt->warn(__MAString__, __VA_ARGS__); \
  } else { \
    PHITOS_NOT_IMPLEMENTED_ASSERT(); \
  }

#define PUSH_LOG_CRITICAL_EXT(__MAString__, ...) \
  if (auto spt = ::opgs16::debug::log::____::Get().lock()) { \
    spt->critical(__MAString__, __VA_ARGS__); \
  } else { \
    PHITOS_NOT_IMPLEMENTED_ASSERT(); \
  }

#define PUSH_LOG_ERROR_EXT(__MAString__, ...) \
  if (auto spt = ::opgs16::debug::log::____::Get().lock()) { \
    spt->error(__MAString__, __VA_ARGS__); \
  } else { \
    PHITOS_NOT_IMPLEMENTED_ASSERT(); \
  }

#else

#define PUSH_LOG_INFO(__string__) ((void*)0)
#define PUSH_LOG_DEBUG(__string__) ((void*)0)
#define PUSH_LOG_WARN(__string__) ((void*)0)
#define PUSH_LOG_CRITICAL(__string__) ((void*)0)
#define PUSH_LOG_ERRO(__string__) ((void*)0)

#define PUSH_LOG_INFO_EXT(__MAString__, ...)  ((void*)0)
#define PUSH_LOG_DEBUG_EXT(__MAString__, ...) ((void*)0)
#define PUSH_LOG_WARN_EXT(__MAString__, ...)  ((void*)0)
#define PUSH_LOG_CRITICAL_EXT(__MAString__, ...) ((void*)0)
#define PUSH_LOG_ERROR_EXT(__MAString__, ...) ((void*)0)

#endif /// opgs16::debug::PushLog only on _DEBUG

#endif /// OPGS16_HEADER_IMPORT_LOGGER_H