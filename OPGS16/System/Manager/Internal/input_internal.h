#ifndef SYSTEM_MANAGER_INTERNAL_INPUT_INTERNAL_H
#define SYSTEM_MANAGER_INTERNAL_INPUT_INTERNAL_H

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
 * @file System/Manager/Internal/input_internal.h
 * @brief Input internal type used in InputManager but not used outside.
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-03 Create file.
 */

#include <string>   /*! std::string */

namespace opgs16 {
namespace manager {
namespace _internal {

/*!
 * @struct BindingKeyInfo
 * @brief Stores key info to use in this game, and overall application.
 * You can bind arbitary (Supported on GLFW) key into key container, and get values from
 * them to call input_event handler.
 *
 * If the user presses negative key, key information instance bound to key container
 * set value to -1.0f. Inversely as positive key is.
 * And the user releases negative or positive key, key information instance let values
 * go to neurtal zone. (within range of dead_zone)
 */
struct BindingKeyInfo {
    using negative_button = int;
    using positive_button = int;

    /**
     * @enum KeyInputStatus
     * @brief Indicates key status of keys.
     */
    enum class KeyInputStatus : int {
        POS_PRESSED = 1,	// If positive key is pressed, key_status will sustain PRESSED.
        NEG_PRESSED = 2,	// If negative key is pressed, key_status will sustain PRESSED.
        RELEASED = 3,	// Changes to RELEASED when released. constraints of Update.
        NEUTRAL = 4,	// If released, and within range of dead_zone, NEURTAL.
    };

    enum class KeyType {
        NONE,
        KEYBOARD,
        MOUSE,
        JOYSTICK
    };

    KeyType         key_type{ KeyType::NONE };
    KeyInputStatus  key_status{ KeyInputStatus::NEUTRAL };
    bool			stick_key{ true };			// If true, pressed key sends signal only once.
    mutable bool	send_signal{ false };

    std::string		name{};	// container key name must be same as structure's name. (Camel)
    negative_button neg{ -1 };	// Negative button key information.
    negative_button pos{ -1 };	// Positive button key information.
    float			neutral_gravity{ 1000.f };	// Gravity to drag value down to neutral zone.
    float			dead_zone{ 0.001f };		// The range of neutral zone.
    float			value{ 0.f };				// Value moves along with neg/pos/gravity.
};

inline BindingKeyInfo::KeyType GetKeyType(const std::string& token) {
    if (token == "KB")      return BindingKeyInfo::KeyType::KEYBOARD;
    else if (token == "MS") return BindingKeyInfo::KeyType::MOUSE;
    else if (token == "JS") return BindingKeyInfo::KeyType::JOYSTICK;
    else return BindingKeyInfo::KeyType::NONE;
}

} /*! opgs16::manager::_internal */
} /*! opgs16::manager */
} /*! opgs16 */

#endif // !SYSTEM_MANAGER_INTERNAL_INPUT_INTERNAL_H
