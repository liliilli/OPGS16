
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file Core\entry.cc
/// @brief The file which operates actual application.
/// @author Jongmin Yun
///
/// @log
/// 2018-03-01 Refactoring.
/// 2018-04-07 Move file to System/Core/Private directory.
///

/*! This statements are for checking memory leaks. */
//#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>
//_crtBreakAlloc = 180;
//_CrtDumpMemoryLeaks();

#include <Core\application.h>   /// ::opgs16::MApplication

// ReSharper disable CppInconsistentNaming
int main() {
    auto& app = opgs16::MApplication::Instance();
    app.Initiate();
    app.Run();

    return 0;
}
