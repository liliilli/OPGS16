#ifndef OPGS16_HELPER_STRING_HELPER_H
#define OPGS16_HELPER_STRING_HELPER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/string_helper.h
///
/// @brief
/// Json helper class.
///
/// @author Jongmin Yun
/// @log
/// 2018-06-17 Create file.
///

#include <string>

namespace opgs16::helper {

///
/// @brief Concatenate directory and files.
///
/// @param[in] dir Directory path.
/// This path does not have to be ended with '/', '\\'.
///
/// @param[in] file Local file path.
///
std::string ConcatDirectoryWithFile(const std::string& dir,
                                    const std::string& file);

} /// ::opgs16::helper

#endif /// OPGS16_HELPER_STRING_HELPER_H