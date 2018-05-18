#ifndef OPGS16_SYSTEM_HELPER_PUBLIC_FIXED_STACK_H
#define OPGS16_SYSTEM_HELPER_PUBLIC_FIXED_STACK_H

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
 * @file System/Helper/Public/fixed_stack.h
 * @brief Stack container have fixed limits.
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Create file.
 */

#include <cassert>
#include <utility>

namespace opgs16 {
/*! Custom type */
namespace cu_type {

template <class _Ty, size_t _Limit>
class FixedStack {
public:
    constexpr void Push(const _Ty& value) {
        assert(m_count < _Limit - 1);
        m_container[m_count] = value;
        ++m_count;
    }

    constexpr _Ty& Top() const {
        return m_container[m_count];
    }

    constexpr void Pop() {
        if (m_count > 0) {
            m_container[m_count].~Ty();
            m_count -= 1;
        }
    }

    constexpr bool Empty() const noexcept {
        return m_count == 0;
    }

    constexpr void Clean() {
        while (!Empty()) {
            Pop();
        }
    }

private:
    _Ty     m_container[_Limit] { _Ty{}, };
    size_t  m_count{};
};

template <class _Ty, size_t _Limit>
inline constexpr void Clean(FixedStack<_Ty, _Limit>& fixed_stack) {
    while (!fixed_stack.Empty()) {
        fixed_stack.Pop();
    }
}

} /*! opgs16::cu_type */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_HELPER_PUBLIC_FIXED_STACK_H

