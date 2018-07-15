#ifndef SYSTEM_HELPER_PUBLIC_HASH_FUNCTION_H
#define SYSTEM_HELPER_PUBLIC_HASH_FUNCTION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/hash_function.h
///
/// @brief Simple but calculated in compile time hash function replaces std::hash().
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-07 Make file.
///
/// @reference http://www.sysnet.pe.kr/Default.aspx?mode=2&sub=0&detail=1&pageno=0&wid=1222&rssMode=1&wtype=0
/// https://handmade.network/forums/t/1507-compile_time_string_hashing_with_c++_constexpr_vs._your_own_preprocessor
///

#define H1(s,i,x)   (x*0xEDB88320+(uint8_t)s[(i)<(sizeof(s)-1)?(sizeof(s)-2-(i)):(sizeof(s)-1)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

/*! USE THIS MACROS */

#if _MSC_VER
#define HASH(s) \
    __pragma( warning(push) ) \
    __pragma( warning(disable:4307) ) \
    ((uint32_t)(H256(#s,0,0))) \
    __pragma( warning(pop) )
#else
#define HASH(s) \
    ((uint32_t)(H256(#s,0,0)))
#endif

#define HASH_STR(s) ((uint32_t)(H256(s,0,0)))

#endif /*! SYSTEM_HELPER_PUBLIC_HASH_FUNCTION_H */