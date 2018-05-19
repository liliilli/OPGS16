#ifndef OPGS16_HELPER_OPGS_ASSERT_H
#define OPGS16_HELPER_OPGS_ASSERT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/assert.h
///
/// @brief
/// Enhanced assertion implementation header file.
///
/// These assertions are used to replace with generic assert(expression)
/// providing more information and features such as logging.
///
/// The user can use assertions with third party logger or just iostream.
/// 
/// @author Jongmin Yun
///
/// @log
/// 2018-05-20 Create file.
///
/// @link
/// https://stackoverflow.com/questions/12062365/is-using-assert-in-c-bad-practice
///
/// @todo Check if console window is opened or not.
/// @todo Implement third logger library message printing.
/// @todo Show stack trace in debug mode.
///

#if defined(false)
#define USE_THIRD_PARTY_LOGGER
#endif

#ifndef USE_THIRD_PARTY_LOGGER
#include <iostream>
#else
#include <Headers\import_logger.h>
#endif

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Macroes
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

#ifndef NDEBUG
#define NEU_ASSERT(__MAExpr__, __MAMessage__) \
  ::opgs16::debug::__EnhancedAssert(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, __MAMessage__)
#else
#define NEU_ASSERT(__MAExpr__, __MAMessage__) \
  (void(0));
#endif

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Implementation 
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

namespace opgs16::debug {
 
///
/// @brief
/// Test expression on runtime.
/// If expression return false, abort() with failed_message unlike assert().
///
/// @link 
/// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
///
inline void __EnhancedAssert(
    const char* expression_string, 
    bool expression_result, 
    const char* file_path, 
    int line, 
    const char* failed_message) {
  if (!expression_result) {
#ifndef USE_THIRD_PARTY_LOGGER
    std::cerr << "Assert failed : " << failed_message << "\n"
        << "Expected : \t" << expression_string << "\n"
        << "Source : \t" << file_path << ", line " << line << "\n";
#else
    PUSH_LOG_ERRO("Assertion failed blahblah...");
#endif
    abort();
  }
}
 
} /// ::opgs16::debug

#endif /// OPGS16_HELPER_OPGS_ASSERT_H