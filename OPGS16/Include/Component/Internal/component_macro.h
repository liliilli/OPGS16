#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/component_macro.h
/// @brief Preprocessor meta programming impelmentation file.
/// @author Jongmin Yun
/// @log
/// 2018-03-07 Correction.
///

#include <Helper/hash_crc32.h>

#define TO_STRING(__TYPE__) #__TYPE__   /*! Convert arguement to string literal */

///
/// @macro SET_UP_HASH_VALUE
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
///
#define SET_UP_HASH_VALUE(__TYPE__) \
public: \
static constexpr const char* __string_literal = TO_STRING(__TYPE__); \
static constexpr uint32_t type = opgs16::__hash::ToCrc32(__string_literal);

/*!
 * @macro OVERRIDE_TYPEMATCH
 * @brief Define override function of component::CComponent's DoesTypeMatch() to match with
 * new type value of __DERIVED__ type. Defined function does not throw exception.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__) \
public: \
inline virtual bool DoesTypeMatch( \
    const uint32_t type_val, \
    const char* str) const noexcept override { \
  if (__DERIVED__::type == type_val && \
      strcmp(str, __DERIVED__::__string_literal) == 0) \
    return true; \
  else return __BASE__::DoesTypeMatch(type_val, str); \
}

/*!
 * @macro SET_UP_TYPE_MEMBER
 * @brief Automaticallly create and define class members related to hashed type value, together.
 *
 * @param[in] __BASE__ Base type of __DERIVED__ type.
 * The argument has to be a full name, with namespace.
 * @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
 */
#define SET_UP_TYPE_MEMBER(__BASE__, __DERIVED__) \
SET_UP_HASH_VALUE(__DERIVED__) \
OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_MACRO_H
