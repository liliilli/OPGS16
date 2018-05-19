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

#define USE_THIRD_PARTY_LOGGER

#ifndef USE_THIRD_PARTY_LOGGER
#include <iostream>
#else
#include <Headers\import_logger.h>
#endif

#ifndef NDEBUG
#define NEU_ASSERT(__MAExpr__, __MAMessage__) \
  __EnhancedAssert(#__MAExpr__, __MAExpr__, __FILE__, __LINE__, __MAMessage__)
#else
#define NEU_ASSERT(__MAExpr__, __MAMessage__) \
  (void(0));
#endif

void __EnhancedAssert(const char* expression_string, 
                      bool expression_result,
                      const char* file_path,
                      int line,
                      const char* failed_message) {
  if (!expression_result) {
#ifndef USE_THIRD_PARTY_LOGGER

#else

#endif
  }
}

#endif /// OPGS16_HELPER_OPGS_ASSERT_H