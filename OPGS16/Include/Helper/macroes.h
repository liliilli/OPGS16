#ifndef OPGS16_HELPER_MACORES_H
#define OPGS16_HELPER_MACORES_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/macroes.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-02 Create file
/// 2018-08-07 Add something.
/// 2018-08-09 Move component_macro.h to macroes.h
///

#include <Helper/hash_crc32.h>

namespace opgs16 {

///
/// @brief Convert arguement to string literal
///
#define OP16_TOSTRING(__MAString__) \
  #__MAString__

#define OP16_SAFE_DELETE(__MAInstance__) \
  delete __MAInstance__; \
  __MAInstance__ = nullptr

#define OP16_GET_LITERAL_NAME(__MAInstance__) \
  __MAInstance__::s_class_name

#define OP16_CLASS_LITERAL_NAME(__MAInstance__) \
  static constexpr const char* s_class_name = OP16_TOSTRING(__MAInstance__)

///
/// @macro OP16_SETUP_HASH
/// @brief Declare type member variable, which is used for storing hashed type value.
/// and value it in compile time.
///
#define OP16_SETUP_HASH(__TYPE__) \
public: \
static constexpr const char* __string_literal = OP16_TOSTRING(__TYPE__); \
static constexpr uint32_t __hash_val = opgs16::__hash::ToCrc32(__string_literal);

///
/// @macro OVERRIDE_TYPEMATCH
/// @brief Define override function of component::CComponent's DoesTypeMatch() to match with
/// new type value of __DERIVED__ type. Defined function does not throw exception.
///
/// @param[in] __BASE__ Base type of __DERIVED__ type.
/// The argument has to be a full name, with namespace.
/// @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
///
#define OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__) \
public: \
inline virtual bool DoesTypeMatch( \
    const uint32_t type_val, \
    const char* str) const noexcept override { \
  if (__DERIVED__::__hash_val == type_val && \
      strcmp(str, __DERIVED__::__string_literal) == 0) \
    return true; \
  else return __BASE__::DoesTypeMatch(type_val, str); \
}

///
/// @macro SET_UP_TYPE_MEMBER
/// @brief Automaticallly create and define class members related to hashed type value, together.
///
/// @param[in] __BASE__ Base type of __DERIVED__ type.
/// The argument has to be a full name, with namespace.
/// @param[in] __DERIVED__ __DERIVED__ type itself. The argument doesn't have to be a full name.
///
#define SET_UP_TYPE_MEMBER(__BASE__, __DERIVED__) \
OP16_SETUP_HASH(__DERIVED__) \
OVERRIDE_TYPEMATCH(__BASE__, __DERIVED__)


} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_MACORES_H