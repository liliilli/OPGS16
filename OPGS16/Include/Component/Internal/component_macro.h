#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H

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
 * @file Components/Internal/component_macro.h
 * @brief Preprocessor meta programming impelmentation file.
 * @author Jongmin Yun
 * @log
 * 2018-03-07 Correction.
 */

#include <Helper\hash_function.h>   /// HASH(), HASH_STR()

#define TO_STRING(__TYPE__) #__TYPE__   /*! Convert arguement to string literal */

/*!
 * @macro SET_UP_HASH_VALUE
 * @brief Declare type member variable, which is used for storing hashed type value.
 * and value it in compile time.
 */
#define SET_UP_HASH_VALUE(__TYPE__) \
public: static constexpr uint32_t type { HASH(__TYPE__) };

/*!
 * @macro OVERRIDE_TYPEMATCH
 * @brief Define override function of component::CComponent's DoesTypeMatch() to match with
 * new type value of __DERIVED__ type. Defined function does not throw exception.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)                                       \
public:                                                                                 \
inline virtual bool DoesTypeMatch(const uint32_t type_val) const noexcept override {    \
    if (__DERIVED__::type == type_val)                                                  \
        return true;                                                                    \
    else return __BASE__::DoesTypeMatch(type_val);                                      \
}

/*!
 * @macro SET_UP_TYPE_MEMBER
 * @brief Automaticallly create and define class members related to hashed type value, together.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define SET_UP_TYPE_MEMBER(__BASE__, __DERIVED__)                                       \
SET_UP_HASH_VALUE(__DERIVED__)                                                          \
OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H
