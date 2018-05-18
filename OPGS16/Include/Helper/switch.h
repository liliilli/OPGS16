#ifndef OPGS16_SYSTEM_HELPER_SWITCH_H
#define OPGS16_SYSTEM_HELPER_SWITCH_H

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
 * @file System/Helper/Public/switch.h
 * @brief Switch enum class replaces plain boolean type.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-01 Create file.
 */

namespace opgs16 {

/*! Switch enum constant used everywhere instead of just using plain bool type. */
enum class Switch : bool {
    OFF = false,
    ON = true
};

/*! Toggle switch value helper function. */
inline Switch ToggleSwitch(const Switch value) noexcept {
    return ((value == Switch::OFF) ? Switch::ON : Switch::OFF);
}

/*! Return boolean value whether state of switch value is ON. */
inline constexpr bool IsSwitchOn(const Switch value) {
    return value == Switch::ON;
}

}

#endif /*! OPGS16_SYSTEM_HELPER_SWITCH_H */
