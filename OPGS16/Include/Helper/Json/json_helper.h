#ifndef OPGS16_HELPER_JSON_JSON_HELPER_H
#define OPGS16_HELPER_JSON_JSON_HELPER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Json/json_helper.h
///
/// @brief
/// Json helper class.
///
/// @author Jongmin Yun
/// @log
/// 2018-06-17 Create file.
///

#include <nlohmann/json_fwd.hpp>

#include <Phitos/Enums/success.h>
#include <Phitos/Enums/found.h>

namespace opgs16::helper::json {

///
/// @brief Find if there is a key in specified json instance.
///
/// @param[in] json Json instance.
/// @param[in] key String key to find.
///
/// @return found flag.
///
phitos::enums::EFound IsJsonKeyExist(const nlohmann::json& json,
                                     const std::string& key);

} /// ::opgs16::helper::json namespace

#endif /// OPGS16_HELPER_JSON_JSON_HELPER_H