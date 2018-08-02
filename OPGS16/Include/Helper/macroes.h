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
///

namespace opgs16 {

#define OP16_SAFE_DELETE(__MAInstance__) \
  delete __MAInstance__; \
  __MAInstance__ = nullptr

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_MACORES_H