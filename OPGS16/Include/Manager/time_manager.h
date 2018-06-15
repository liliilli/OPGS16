#ifndef OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/time_manager.h
///
/// @brief Time manager manages time quantums, fps, and so forth.
///
/// @log
/// 2018-02-10 Create file.
/// 2018-03-03 Refactoring.
/// 2018-06-15 Refactoring.
///

/// ::phitos::enums::ESwitch
#include <Phitos/Enums/switch.h>

///
/// @namespace time
///
/// @brief Manages time quantum.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-03 Refactoring. Moved it to opgs16::manager namespace.
/// 2018-06-15 Refactoring.
///
namespace opgs16::manager::time {

///
/// @brief Update time tick for application.
///
void Update();

///
/// @brief Helper method that checks if or not next frame can be proceeded.
/// @return If next frame is set, return true. else false.
///
bool Ticked();

///
/// @brief Get delta time how much time passed from previous frame.
/// Format is integer part is seconds, and point part is millisecond region.
///
/// @return Float value delta time value.
///
float GetDeltaTime();

///
/// @brief Return present frame per second.
///
float GetPresentFps();

///
/// @brief The methods set fps (frame per seconds);
/// This methods can be called anytime use wants.
/// But when v-sync is off, this methods would be useless.
///
/// @param[in] fps_value Fps tick count per second.
///
void SetFps(float fps_value);

///
/// @brief Set vsync mode.
/// If vsync mode is off, application will be running by more 60 fps but unstable.
/// If vsync mode is on, it will be running by your specified fps or 60 fps.
///
/// @param[in] mode Mode switch value.
///
void SetVsync(phitos::enums::ESwitch mode);

} /// ::opgs16::manager::time namespace

#endif // !OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H