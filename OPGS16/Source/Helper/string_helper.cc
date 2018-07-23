#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/string_helper.cc
///
/// @brief Json helper class definition file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-17 Create file.
///

/// Header file
#include <Helper/string_helper.h>

namespace opgs16::helper {

std::string ConcatDirectoryWithFile(const std::string& dir,
                                    const std::string& file) {
  // Temporary enum (weak enum) type.
  enum : char {
    Back = '\\',
    Forward = '/'
  } slash_mode = Back;

  std::string return_path_string = dir;

  if (dir.find(Back) == std::string::npos) {
    slash_mode = Forward;
  }

  if (dir.find_last_of(slash_mode) == (dir.length() - 1)) {
    return_path_string.append(file);
  }
  else {
    return_path_string += slash_mode;
    return_path_string.append(file);
  }

  return return_path_string;
}

} /// ::opgs16::helper namespace