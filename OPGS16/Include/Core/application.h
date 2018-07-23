#ifndef OPGS16_CORE_APPLICATION_H
#define OPGS16_CORE_APPLICATION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/application.h
///
/// @brief
/// Main file to run application, which consists of application operation class.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-01 Done refactoring following "studioN coding style convention".
/// 2018-03-11 Corection of ::opgs16::element::CObject class.
/// 2018-05-19 Change singleton structure to namespace structure.
///

#include <functional>

/// Forward declaration
#include <opgs16fwd.h>

///
/// @namespace opgs16::entry
/// @brief
/// core entry namespace which consists of update, draw functions.
///
/// @log
/// 2018-03-01
/// Move class inside opgs16 namespace for uniformation.
/// Refactored and removed member functions are not appropriate for class.
/// 2018-05-19 Change singleton structure to namespace structure. yay!
///
namespace opgs16::entry {

///
/// @brief
/// Let initiate game setting to operate application.
/// This function must be called at first of entry function.
///
void Initiate();

///
/// @brief
/// Let application run.
///
void Run();

///
/// @brief
///
void ExitGame();

///
/// @brief
/// Set callback function will be called before update frame.
///
/// @param callback
/// Callback function to execute at next frame.
///
void SetOnBeforeUpdateCallback(std::function<void(void)> callback);

} /// opgs16::entry

#endif /// OPGS16_CORE_APPLICATION_H
