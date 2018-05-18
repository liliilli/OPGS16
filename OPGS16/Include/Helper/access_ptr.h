#ifndef OPGS16_SYSTEM_HELPER_ACCESS_PTR_H
#define OPGS16_SYSTEM_HELPER_ACCESS_PTR_H

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
 * @file System/Helper/Public/access_ptr.h
 * @author Jongmin Yun
 * @log
 * 2018-03-03 Make fundamental access_ptr type. This type could only permits class type.
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>

template <
    class _Ty,
    typename = std::enable_if_t<
        !std::is_null_pointer_v<_Ty> &&
        std::is_class_v<_Ty>
    >
>
class access_ptr final {
public:
    using element_type = _Ty;

    constexpr access_ptr() : m_ptr{nullptr} {};
    constexpr access_ptr(nullptr_t) : access_ptr() {};
    constexpr access_ptr(element_type* const ptr) : m_ptr{ptr} {};
    constexpr access_ptr(const std::unique_ptr<element_type>& ptr) : m_ptr{ptr.get()} {};

    friend std::ostream& operator<<(std::ostream& stream,
                                    const access_ptr<element_type>& value) {
        stream << value.m_ptr;
        return stream;
    }

    constexpr bool empty() const noexcept {
        return m_ptr == nullptr;
    }
    constexpr void release() noexcept {
        assert(m_ptr != nullptr);
        m_ptr = nullptr;
    }

    constexpr element_type* operator->() const noexcept { return m_ptr; }
    constexpr access_ptr<element_type>& operator=(const access_ptr<element_type>& rhs) {
        m_ptr = rhs.m_ptr;
        return *this;
    }
    constexpr access_ptr<element_type>& operator=(const std::unique_ptr<element_type>& ptr) {
        assert(ptr != nullptr);
        m_ptr = ptr.get();
        return *this;
    }

private:
    element_type* m_ptr;
};

#endif /*! OPGS16_SYSTEM_HELPER_ACCESS_PTR_H */