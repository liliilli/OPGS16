#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include <Core/core_setting.h>
#include <../manifest.h>

namespace {
using opgs16::setting::TActualScreenSize;

bool m_anti_aliasing_feature    = false;
bool m_post_processing_feature  = false;
bool m_render_collision_aabb    = false;

bool m_is_debug_mode =
#if defined(OP16_SETTING_DEBUG_MODE)
  true;
#else
  false;
#endif

EScaleType m_scale_value = EScaleType::X1;

bool m_scaling_feature =
#if defined(OP16_SETTING_RESOLUTION_640480)
  false;
#else
  true;
#endif

constexpr bool m_logging_feature =
#if defined(OP16_SETTING_ENABLE_LOGGING)
  true;
#else
  false;
#endif

constexpr bool m_logging_enable_console =
#if defined(OP16_SETTING_ENABLE_LOGGING_TO_CONSOLE)
  true;
#else
  false;
#endif

constexpr bool m_logging_enable_file =
#if defined(OP16_SETTING_ENABLE_LOGGING_TO_FILE)
  true;
#else
  false;
#endif

constexpr std::string_view m_logging_file_path =
#if defined(OP16_SETTING_LOGGING_FILE_PATH)
  OP16_SETTING_LOGGING_FILE_PATH;
#else
  "log.txt";
#endif

// Screen size
#if defined(OP16_SETTING_RESOLUTION_256224)
  #if defined(OP16_SETTING_RESOLUTION_320240) ||\
      defined(OP16_SETTING_RESOLUTION_640480) ||\
      defined(OP16_SETTING_RESOLUTION_400240)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 256, 224 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_320240)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_400240) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 320, 240 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_400240)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_320240) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 400, 240 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_640480)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_320240) ||\
      defined(OP16_SETTING_RESOLUTION_400240)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 640, 480 };
  #endif
#else
  static_assert(false, "At least one resolution setting option must be turned on.")
#endif

// Vsync fixed fps
#if defined(OP16_SETTING_USING_60FPS_ON_VSYNC)
  #if defined(OP16_SETTING_USING_30FPS_ON_VSYNC)
    static_assert(false, "Just only one vsync fixed fps setting must be turned on.");
  #else
    constexpr int32_t s_vsync_fixed_fps = 60;
  #endif
#elif defined(OP16_SETTING_USING_30FPS_ON_VSYNC)
  #if defined(OP16_SETTING_USING_60FPS_ON_VSYNC)
    static_assert(false, "Just only one vsync fixed fps setting must be turned on.");
  #else
    constexpr int32_t s_vsync_fixed_fps = 30;
  #endif
#else
  static_assert(false, "At least one vsync fixed fps option must be turned on.");
#endif
} /// ::unnamed namesapce

namespace opgs16::setting {

bool IsEnableAntiAliasing() noexcept {
  return m_anti_aliasing_feature;
}

bool IsEnableDebugMode() noexcept {
  return m_is_debug_mode;
}

bool IsEnablePostProcessing() noexcept {
  return m_post_processing_feature;
}

bool IsEnableScaling() noexcept {
  return m_scaling_feature;
}

bool IsEnableRenderingAabb() noexcept {
  return m_render_collision_aabb;
}

bool IsEnableLoggingFeature() noexcept {
  return m_logging_feature;
}

bool IsEnableLoggingConsole() noexcept {
  return m_logging_enable_console;
}

bool IsEnableLoggingToFile() noexcept {
  return m_logging_enable_file;
}

const std::string_view& GetLoggingFileRelativePath() noexcept {
  return m_logging_file_path;
}

EScaleType GetScaleValue() noexcept {
  return m_scale_value;
}

int32_t GetScaleValueIntegerOf() noexcept {
  return static_cast<int32_t>(m_scale_value);
}

void SetAntiAliasing(Switch value) noexcept {
  m_anti_aliasing_feature = value == Switch::ON;
}
void SetDebugMode(Switch value) noexcept {
  m_is_debug_mode = value == Switch::ON;
}
void SetPostProcessing(Switch value) noexcept {
  m_post_processing_feature = value == Switch::ON;
}
void SetSizeScalable(Switch value) noexcept {
  m_scaling_feature = value == Switch::ON;
}
void SetScaleValue(EScaleType value) noexcept {
  m_scale_value = value;
}
void SetCollisionAABBBoxDisplay(Switch value) noexcept {
  m_render_collision_aabb = value == Switch::ON;
}

void ToggleAntiAliasing() noexcept {
  SetAntiAliasing(ToggleSwitch(Switch{IsEnableAntiAliasing()}));
}
void TogglePostProcessing() {
  SetPostProcessing(ToggleSwitch(Switch{IsEnablePostProcessing()}));
}
void ToggleSizeScalable() {
  SetSizeScalable(ToggleSwitch(Switch{IsEnableScaling()}));
}
void ToggleCollisionAABBBoxDisplay() noexcept {
  SetCollisionAABBBoxDisplay(ToggleSwitch(Switch{IsEnableRenderingAabb()}));
}

const TActualScreenSize& GetScreenSize() noexcept {
  return screen_size;
}

TActualScreenSize::value_type GetScreenWidth() noexcept {
  return screen_size[0];
}

TActualScreenSize::value_type GetScreenHeight() noexcept {
  return screen_size[1];
}

int32_t GetFixedFpsValue() noexcept {
  return s_vsync_fixed_fps;
}

} /// ::opgs16