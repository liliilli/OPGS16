#ifndef OPGS16_SYSTEM_CORE_CORE_SETTING_H
#define OPGS16_SYSTEM_CORE_CORE_SETTING_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file Core/core_setting.h
 * @brief Have been written about application global setting components and management class.
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-01 Create file.
 */

#include <array>        /*! std::array */
#include <type_traits>  /*! std::underlying_type<> */

#include <Helper\switch.h>  /// ::opgs16::Switch

#define _OPGS16_VERSION_MAJOR 0
#define _OPGS16_VERSION_MINOR 1
#define _OPGS16_VERSION_FIXED 0
#define _OPGS16_CODENAME Dragonload

namespace opgs16 {

enum class EScaleType : unsigned {
    X1 = 1, /*! Screen will be shown with screen size */
    X2 = 2,	/*! Screen will be shown with double of screen size */
    X3 = 3,	/*! Screen will be shown with triple of screen size */
};

/*! Check and return boolean value whether lhs is same to rhs. */
inline constexpr bool IsSameValue(EScaleType lhs, EScaleType rhs) {
    return lhs == rhs;
}

/*!
 * @class SGlobalSetting
 * @brief Storage of application global setting. these setting options are provided by default,
 * so user could not insert new option or remove existed option.
 *
 * Example : get screen size or either width or height.
 *     auto var = opgs16::SGlobalSetting::ScreenSize();
 *     auto var = opgs16::SGlobalSetting::ScreenWidth();
 *     auto var = opgs16::SGlobalSetting::ScreenHeight();
 */
class SGlobalSetting final {
private:
    using size_type = std::array<unsigned, 2>;

public:
    constexpr static size_type ScreenSize() noexcept {
        return screen_size;
    }
    constexpr static size_type::value_type ScreenWidth() noexcept {
        return screen_size[0];
    }
    constexpr static size_type::value_type ScreenHeight() noexcept {
        return screen_size[1];
    }

    inline Switch AntiAliasing() const noexcept {
        return m_anti_aliasing;
    }
    inline Switch DebugMode() const noexcept {
        return m_debug_mode;
    }
    inline Switch PostProcessing() const noexcept {
        return m_post_processing;
    }
    inline Switch SizeScalable() const noexcept {
        return m_size_scalable;
    }
    inline EScaleType ScaleValue() const noexcept {
        return m_scale_value;
    }
    inline Switch CollisionAABBBoxDisplay() const noexcept {
      return m_collision_aabb_box;
    }
    inline auto ScaleValueIntegerOf() const noexcept {
        return static_cast<std::underlying_type_t<decltype(m_scale_value)>>(m_scale_value);
    }

    inline void SetAntiAliasing(Switch value) noexcept {
        m_anti_aliasing = value;
    }
    inline void SetDebugMode(Switch value) noexcept {
        m_debug_mode = value;
    }
    inline void SetPostProcessing(Switch value) noexcept {
        m_post_processing = value;
    }
    inline void SetSizeScalable(Switch value) noexcept {
        m_size_scalable = value;
    }
    inline void SetScaleValue(EScaleType value) noexcept {
        m_scale_value = value;
    }
    inline void SetCollisionAABBBoxDisplay(Switch value) noexcept {
      m_collision_aabb_box = value;
    }

    inline void ToggleAntiAliasing() noexcept {
        SetAntiAliasing(ToggleSwitch(AntiAliasing()));
    }
    inline void ToggleDebugMode() noexcept {
        SetDebugMode(ToggleSwitch(DebugMode()));
    }
    inline void TogglePostProcessing() {
        SetPostProcessing(ToggleSwitch(PostProcessing()));
    }
    inline void ToggleSizeScalable() {
        SetSizeScalable(ToggleSwitch(SizeScalable()));
    }
    inline void ToggleCollisionAABBBoxDisplay() noexcept {
      SetCollisionAABBBoxDisplay(ToggleSwitch(CollisionAABBBoxDisplay()));
    }

private:
    constexpr static size_type screen_size{ 256, 224 };

    Switch m_anti_aliasing{ false };
    Switch m_debug_mode{ false };
    Switch m_post_processing{ false };
    Switch m_size_scalable{ true };
    Switch m_collision_aabb_box = Switch::OFF;
    EScaleType m_scale_value{ EScaleType::X1 };

public:
    SGlobalSetting();
    SGlobalSetting(const SGlobalSetting&) = delete;
    SGlobalSetting operator=(const SGlobalSetting&) = delete;
};

} /*! opgs16 */

#endif

