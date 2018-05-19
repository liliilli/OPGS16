#ifndef OPGS16_CORE_INTERNAL_APPLICATION_STATUS_H
#define OPGS16_CORE_INTERNAL_APPLICATION_STATUS_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/Internal/application_status.h
///
/// @brief
/// This file implements internal game application status type. 
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-19 
/// Create file, move GameStatus enum type into this.
/// Rename GameStatus to EGameStatus following naming convention.
///

namespace opgs16::entry::_internal {

///
/// @enum EGameStatus
/// @brief 
/// Global game status in this game application.
///
enum class EGameStatus {
  INIT,	    // Initial status in game application.
  MENU,	    // Global menu.
  PLAYING,  // Actual play mode, not paused, not menu.
  EXIT,	    // Exit process from game application returning to select menu.
  TERMINATE
};

} /// ::opgs16::entry::_internal

#endif /// OPGS16_CORE_INTERNAL_APPLICATION_STATUS_H