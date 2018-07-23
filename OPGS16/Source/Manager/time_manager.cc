#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/time_manager.cc
///
/// @brief Time manager manages time quantums, fps, and so forth.
///
/// @log
/// 2018-06-15 Create file.
///

/// Header file
#include <Manager/time_manager.h>

#include <GLFW/glfw3.h>
/// ::phitos::enums::ESwitch
#include <Phitos/Enums/switch.h>

//!
//! Datas
//!

namespace {
using phitos::enums::ESwitch;

// Old time point of previous frame.
float old_time = 0.f;
// New time point of present frame.
float new_time = 0.f;
// Total time from previous to new, used for checking frame tick.
float elapsed_time = 0.f;

/// Internal second calculation value.
float __elapsed_second = 0.f;

// Delta time of each tick.
float delta_time = 0.f;
// frame tick fixed_fps_interval time, used only v-sync is on.
float fixed_fps_interval = 0.f;

// Time value for displaying text when fps_toggled is true.
float previous_fps_second = 0.f;
float present_fps_second = 0.f;

ESwitch is_first = ESwitch::Off;
ESwitch is_vsync = ESwitch::On;

} /// unnamed namespace

//!
//! Definition
//!

namespace opgs16::manager::time {

void Update() {
  if (is_first == ESwitch::Off) {
    old_time = new_time = static_cast<float>(glfwGetTime());
    is_first = !is_first;
    return;
  }

  new_time = static_cast<float>(glfwGetTime());
  const auto tick = new_time - old_time;

  switch (is_vsync) {
  case ESwitch::Off: elapsed_time = tick; break;
  case ESwitch::On: elapsed_time += tick; break;
  }

  if (__elapsed_second += tick; __elapsed_second >= 1.0f) {
    __elapsed_second -= 1.0f;
    previous_fps_second = present_fps_second;
    present_fps_second = 0.f;
  }

  old_time = new_time;
}

bool Ticked() {
  if (is_vsync == ESwitch::Off) {
    delta_time = elapsed_time;
    present_fps_second += 1;
    return true;
  }

  // Fixed frame per second below.
  if (elapsed_time < fixed_fps_interval)
    return false;

  present_fps_second += 1;
  delta_time    = fixed_fps_interval;
  elapsed_time -= fixed_fps_interval;
  return true;
}

float GetDeltaTime() {
  return delta_time;
}

float GetPresentFps() {
  return previous_fps_second;
}

void SetFps(float fps_value) {
  fixed_fps_interval = 1 / fps_value;
}

void SetVsync(phitos::enums::ESwitch mode) {
  is_vsync = mode;
  elapsed_time = 0.f;
}

} /// ::opgs16::manager::time namespace