#ifndef OPGS16_SYSTEM_HELPER_PUBLIC_TEMPLATE_H
#define OPGS16_SYSTEM_HELPER_PUBLIC_TEMPLATE_H

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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Helper/Public/template.h
 * @brief Template parameter overloading resolution rule test helper file.
 * @author Jongmin Yun
 * @log
 * 2018-03-16 Create file.
 * 2018-04-14 Add ::IsCSceneBase<_Ty>
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <memory>
#include "../../Headers/Fwd/objectfwd.h"    /*! ::opgs16::component::CCamera */

template <class _Ty>
inline constexpr bool IsCObjectBase = std::is_base_of_v<opgs16::element::CObject, _Ty>;

template <class _Ty>
inline constexpr bool IsCObjectSmtPtr = std::is_convertible_v<
    std::unique_ptr<std::decay_t<_Ty>>, std::unique_ptr<opgs16::element::CObject>>;

template <class _Ty>
inline constexpr bool IsCSceneBase = std::is_base_of_v<opgs16::element::CScene, _Ty>;

#endif // !OPGS16_SYSTEM_HELPER_PUBLIC_TEMPLATE_H

