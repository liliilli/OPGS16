
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Json/json_helper.cc
///
/// @brief Json helper class definition file
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-17 Create file.
///

/// Header file
#include <Helper/Json/json_helper.h>

/// json library file
#include <json.hpp>

namespace opgs16::helper::json {

phitos::enums::EFound IsJsonKeyExist(const nlohmann::json& json,
                                 const std::string& key) {
  using phitos::enums::EFound;

  return json.find(key) == json.end() ? EFound::NotFound : EFound::Found;
}

} /// ::opgs16::helper::json namespace