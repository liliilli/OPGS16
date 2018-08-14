#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/entry.cc
///
/// @brief
/// The file has main function that operates actual application.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-01 Refactoring.
/// 2018-04-07 Move file to System/Core/Private directory.
/// 2018-05-19 Change the way of calling application initiation.
///

/*! This statements are for checking memory leaks. */
#ifdef false
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
_crtBreakAlloc = 180;
_CrtDumpMemoryLeaks();
#endif

/// ::opgs16::MApplication
#include <Core/application.h>
#include "Helper/Type/quaternion.h"
#include "Headers/import_logger.h"
#include <Helper/Type/vector3.h>
#include "Helper/Type/axis.h"

int main(int argc, char** argv) {
#if defined(_WIN32)
#if defined(_DEBUG)
  ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
#endif
  opgs16::entry::Initiate();
  opgs16::entry::Run();
  return 0;
}
