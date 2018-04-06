#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_ANIMATOR_INTERNAL_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_ANIMATOR_INTERNAL_H

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
 * @file System/Components/Internal/animator_internal.h
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Create file, Add state enum.
 * 2018-04-06 Change AnimationCell::m_index to AnimationCell::m_fragment_index.
 */

#include <string>   /*! std::string */

namespace opgs16 {
namespace component {
namespace _internal {

enum class AnimatorState {
    START,
    ANIMATION_START,
    UPDATE,
    ANIMATION_END,
    END,
    SLEEP
};

/*!
 * @class AnimationCell
 * @brief animation cell that stores time, texture_name, texture fragment index.
 * This is used for animating.
 * @log
 * 2018-04-06 Add comment, IndexSize to unsigned of m_fragment_index.
 */
class AnimationCell {
    std::string m_texture_name{};
    unsigned    m_fragment_index{ 0 };
    unsigned    m_time_milli{ 0 };

public:
    AnimationCell() {};
    explicit AnimationCell(const std::string& texture_name, unsigned index, unsigned time_milli) :
        m_texture_name{ texture_name }, m_fragment_index{ index }, m_time_milli{ time_milli } {};

    unsigned Index() const noexcept {
        return m_fragment_index;
    }
    const std::string& TextureName() const noexcept {
        return m_texture_name;
    }
    unsigned TimeMilli() const noexcept {
        return m_time_milli;
    }
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_ANIMATOR_INTERNAL_H


