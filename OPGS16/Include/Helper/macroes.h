#ifndef OPGS16_HELPER_MACORES_H
#define OPGS16_HELPER_MACORES_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/macroes.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-02 Create file
/// 2018-08-07 Add something.
///

namespace opgs16 {

#define OP16_TOSTRING(__MAString__) \
  #__MAString__

#define OP16_SAFE_DELETE(__MAInstance__) \
  delete __MAInstance__; \
  __MAInstance__ = nullptr

#define OP16_CLASS_LITERAL_NAME(__MAInstance__) \
  inline static constexpr const char* s_class_name = OP16_TOSTRING(__MAInstance__)

#define OP16_GET_LITERAL_NAME(__MAInstance__) \
  __MAInstance__::s_class_name

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_MACORES_H