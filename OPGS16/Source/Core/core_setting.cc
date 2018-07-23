
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
bool m_is_debug_mode            = false;
bool m_post_processing_feature  = false;
bool m_render_collision_aabb    = false;

EScaleType m_scale_value = EScaleType::X1;

bool m_scaling_feature =
#if defined(OP16_SETTING_RESOLUTION_640480)
  false;
#else
  true;
#endif

#if defined(OP16_SETTING_RESOLUTION_256224)
  #if defined(OP16_SETTING_RESOLUTION_320240) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 256, 224 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_320240)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 320, 240 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_640480)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_320240)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr TActualScreenSize screen_size{ 640, 480 };
  #endif
#else
  static_assert(false, "At least one resolution setting option must be turned on.")
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
void ToggleDebugMode() noexcept {
  SetDebugMode(ToggleSwitch(Switch{IsEnableDebugMode()}));
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

} /// ::opgs16