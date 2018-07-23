#ifndef OPGS16_SYSTEM_CORE_CORE_SETTING_H
#define OPGS16_SYSTEM_CORE_CORE_SETTING_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/core_setting.h
///
/// @brief Have been written about application global setting components and management class.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-01 Create file.
/// 2018-07-23 Version up! 0.2.0
///

#include <array>
/// ::opgs16::Switch
#include <Helper/switch.h>

#define _OPGS16_VERSION_MAJOR 0
#define _OPGS16_VERSION_MINOR 2
#define _OPGS16_VERSION_FIXED 0
#define _OPGS16_CODENAME Malroth

enum class EScaleType : int32_t {
  X1 = 1, /// Screen will be shown with screen size.
  X2 = 2,	/// Screen will be shown with double of screen size.
  X3 = 3,	/// Screen will be shown with triple of screen size.
};

namespace opgs16::setting {

using TActualScreenSize = std::array<int32_t, 2>;

bool IsEnableAntiAliasing() noexcept;

bool IsEnableDebugMode() noexcept;

bool IsEnablePostProcessing() noexcept;

bool IsEnableScaling() noexcept;

bool IsEnableRenderingAabb() noexcept;

void SetAntiAliasing(Switch value) noexcept;

void SetDebugMode(Switch value) noexcept;

void SetPostProcessing(Switch value) noexcept;

void SetSizeScalable(Switch value) noexcept;

void SetScaleValue(EScaleType value) noexcept;

void SetCollisionAABBBoxDisplay(Switch value) noexcept;

void ToggleAntiAliasing() noexcept;

void ToggleDebugMode() noexcept;

void TogglePostProcessing();

void ToggleSizeScalable();

void ToggleCollisionAABBBoxDisplay() noexcept;

const TActualScreenSize& GetScreenSize() noexcept;

TActualScreenSize::value_type GetScreenWidth() noexcept;

TActualScreenSize::value_type GetScreenHeight() noexcept;

int32_t GetScaleValueIntegerOf() noexcept;

EScaleType GetScaleValue() noexcept;

} /// ::opgs16::setting namespace

#endif