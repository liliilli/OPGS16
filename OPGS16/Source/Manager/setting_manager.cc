
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/setting_manager.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-26 Removed redundant move special functions.
/// 2018-03-04 Refactoring.
/// 2018-04-02 std::string to std::wstring in debug log output.
/// 2018-05-20 Detach singleton pattern into namespace.
/// 2018-06-17 Change reading format to json and interpolate rendering and collison.
///
/// @todo refactoring. add comments
///

/// Header file
#include <Manager/setting_manager.h>

#include <fstream>
#include <string>
#include <vector>

/// nlohmann::json library
#include <nlohmann/json.hpp>
/// Enhanced assertion
#include <Phitos/Dbg/assert.h>
/// ::phitos::enums::ESucceed
#include <Phitos/Enums/success.h>

/// Import logger
#include <Headers/import_logger.h>
/// Expanded assertion
#include <Helper/assert.h>
/// ::opgs16::helper string helper functions.
#include <Helper/string_helper.h>
/// ::opgs16::helper::json helper functions.
#include <Helper/Json/json_helper.h>
/// ::opgs16::manager::_internal flags
#include <Manager/Internal/flag.h>
/// ::opgs16::debug error messages.
#include <Manager/Internal/error_message.h>

/// Manifest file
#include <../manifest.h>

//!
//! Member container
//!

///
/// This namespace is integrity check variable container for
/// checking runtime caveats of source code.
///
namespace {
using opgs16::debug::EInitiated;

const char* s_json_tag = "tag";
const char* s_json_rendering = "rendering";
const char* s_json_collision = "collision";
const char* s_json_gameobject = "gameobject";
const char* s_json_list = "list";
const char* s_json_table = "table";

EInitiated m_initiated = EInitiated::NotInitiated;
} /// unnamed namespace

///
/// This namespace stores variables or constexpr variables to be used
/// functions.
///
namespace {

/// Tag list used in Application.
std::vector<std::string> m_tag_list;
/// Render layer list
std::vector<std::string> m_render_layer_list;
/// Collision Layer list.
std::vector<std::string> m_collision_layer_list;

std::vector<std::vector<bool>> m_collision_check_list;

} /// unnamed namespace

//!
//! Implementation part
//!

///
/// @brief Initialize tag list using file stream.
/// This sequence proceed loading synchronously.
///
/// @param[in] tag_json
/// @param[in] tag_list
///
void InitializeTagList(const nlohmann::json& tag_json,
                       std::vector<std::string>& tag_list);

///
/// @brief
/// Initailize layer list using file stream.
///
/// @param[in] rendering_json
/// @param[in] layer_list
///
void InitializeRenderLayerList(const nlohmann::json& rendering_json,
                               std::vector<std::string>& layer_list);

///
/// @brief
/// Initialize collision layer list using file stream.
///
/// @param[in] collision_json
/// @param[in] layer_list
/// @param[in] check_list
///
void InitializeCollisionLayerList(const nlohmann::json& collision_json,
                                  std::vector<std::string>& layer_list,
                                  std::vector<std::vector<bool>>& check_list);

///
/// @brief
///
/// @param[in] json
/// @param[in] file_path
///
/// @return
///
phitos::enums::ESucceed VerifySettingFile(const nlohmann::json& json,
                                          const std::string& file_path);

///
/// @brief
///
/// @param[in] setting_file_path
///
/// @return
///
void ReadSettingFile(const std::string& setting_file_path);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
phitos::enums::ESucceed VerifyRenderingLayerList(const nlohmann::json& json);

///
/// @brief
///
/// @param[in] json
///
/// @return
///
phitos::enums::ESucceed VerifyingCollisionLayerList(const nlohmann::json& json);

//!
//! Implementation of outside API
//!

namespace opgs16::manager::setting {

void Initiate() {
  PHITOS_ASSERT(m_initiated == EInitiated::NotInitiated,
                debug::err_setting_duplicated_init);

  ReadSettingFile(
      helper::ConcatDirectoryWithFile(_APPLICATION_PROJECT_PATH,
      "Setting/setting.meta"));

  m_initiated = EInitiated::Initiated;
}

const std::string& GetTagName(uint32_t i) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  PHITOS_ASSERT(i < m_tag_list.size(), debug::err_setting_tag_oob);

  return m_tag_list[i];
}

const std::string& GetRenderingLayerName(uint32_t i) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  PHITOS_ASSERT(i < m_render_layer_list.size(), debug::err_setting_rendering_oob);

  return m_render_layer_list[i];
}

const TagList& GetTagNameList() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_tag_list;
}

const RenderingLayerList& GetRenderingLayerNameList() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_render_layer_list;
}

uint32_t GetRenderingLayerNameListSize() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return static_cast<uint32_t>(m_render_layer_list.size());
}

const std::string& GetCollisionLayerName(uint32_t i) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  PHITOS_ASSERT(i < m_collision_layer_list.size(),
      debug::err_setting_collision_oob);

  return m_collision_layer_list[i];
}

const CollisionLayerLIst& GetCollisionLayerList() {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_collision_layer_list;
}

bool CollisionLayerCheck(unsigned lhs, unsigned rhs) {
  PHITOS_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  PHITOS_ASSERT(lhs < m_collision_check_list.size(),
      debug::err_setting_collision_check_oob);
  PHITOS_ASSERT(rhs < m_collision_check_list.size(),
      debug::err_setting_collision_check_oob);

  const auto size = m_collision_check_list.size();
  if (lhs > rhs)
    return m_collision_check_list[rhs][size - lhs - 1];
  else
    return m_collision_check_list[lhs][size - rhs - 1];
}

} /// ::opgs16::manager::setting

//!
//! Implementation of global functions.
//!

void ReadSettingFile(const std::string& setting_file_path) {
  std::ifstream stream { setting_file_path, std::ios_base::in };
  if (!stream.good()) {
    PUSH_LOG_CRITICAL_EXT(
        "Failed to find project setting file. [{} : {}]", "Path",
        setting_file_path);
    stream.close();
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  nlohmann::json atlas_json;
  stream >> atlas_json;
  stream.close();

  using phitos::enums::ESucceed;
  if (VerifySettingFile(atlas_json, setting_file_path) == ESucceed::Failed) {
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  InitializeTagList(atlas_json[s_json_tag], m_tag_list);
  InitializeRenderLayerList(atlas_json[s_json_rendering], m_render_layer_list);
  InitializeCollisionLayerList(atlas_json[s_json_collision],
                               m_collision_layer_list, m_collision_check_list);
}

phitos::enums::ESucceed VerifySettingFile(const nlohmann::json& json,
                                          const std::string& file_path) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_tag) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file. [{} : {}]",
        s_json_tag, "Path", file_path);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_rendering) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file. [{} : {}]",
        s_json_rendering, "Path", file_path);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_collision) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file. [{} : {}]",
        s_json_collision, "Path", file_path);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

void InitializeTagList(const nlohmann::json& tag_json,
                       std::vector<std::string>& tag_list) {
  using phitos::enums::EFound;
  using opgs16::helper::json::IsJsonKeyExist;
  if (IsJsonKeyExist(tag_json, s_json_gameobject) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file.", s_json_gameobject);
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  const auto& gameobject_tag_json = tag_json[s_json_gameobject];
  if (gameobject_tag_json.empty()) {
    PUSH_LOG_CRITICAL(
      "Empty gameobject tag list is prohibited. "
      "Please add at least one more tag.");
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  const auto gameobject_tag_list =
      gameobject_tag_json.get<std::vector<std::string>>();
  const auto length = static_cast<uint32_t>(gameobject_tag_list.size());
  for (uint32_t i = 0; i < length; ++i) {
    PUSH_LOG_DEBUG_EXT("Gameobject [{} : {}]", i, length);
    tag_list.emplace_back(gameobject_tag_list[i]);
  }
}

void InitializeRenderLayerList(const nlohmann::json& rendering_json,
                               std::vector<std::string>& layer_list) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (VerifyRenderingLayerList(rendering_json) == ESucceed::Failed) {
    PUSH_LOG_ERRO("Failed to load rendering layer list.");
    PHITOS_UNEXPECTED_BRANCH();
    return;
  }

  const auto rendering_layer_list =
      rendering_json[s_json_list].get<std::vector<std::string>>();
  const auto length = static_cast<uint32_t>(rendering_layer_list.size());
  for (uint32_t i = 0; i < length; ++i) {
    PUSH_LOG_DEBUG_EXT("Rendering layer [{} : {}]", i, length);
    layer_list.emplace_back(rendering_layer_list[i]);
  }
}

phitos::enums::ESucceed VerifyRenderingLayerList(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file.", s_json_list);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (json[s_json_list].empty()) {
    PUSH_LOG_CRITICAL(
        "Empty rendering list is prohibited. Please add at least one more tag.");
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

void InitializeCollisionLayerList(const nlohmann::json& collision_json,
                                  std::vector<std::string>& layer_list,
                                  std::vector<std::vector<bool>>& check_list) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (VerifyingCollisionLayerList(collision_json) == ESucceed::Failed) {
    PUSH_LOG_ERRO("Failed to load collision layer list.");
    PHITOS_NOT_IMPLEMENTED_ASSERT();
    return;
  }

  // COLLISION CHECK TABLE
  // [ ][0][1][2][3]
  // [0][ ][x][x][ ]
  // [1]___[x][x][x]
  // [2]______[ ][ ]
  // [3]_________[ ]

  for (const auto& element : collision_json[s_json_list])
    layer_list.emplace_back(element.get<std::string>());

  // Make table
  const auto size = layer_list.size();
  check_list.resize(size);
  for (auto i = size; i > 0; --i)
    check_list[size - i].resize(i);

  // Set collision table
  uint32_t i = 0;
  for (const auto& element : collision_json[s_json_table]) {
    auto list = element.get<std::vector<int>>();
    if (list.empty()) {
      ++i;
      continue;
    }

    for (const auto& index : list)
      check_list[i][size - index - 1] = true;
    ++i;
  }

#if defined(_DEBUG)
  for (const auto& item : layer_list) {
    PUSH_LOG_INFO(item.c_str());
  }

  for (const auto& y : check_list) {
    std::string line{};
    for (const auto& x : y) {
      line.append((x == true ? "1 " : "0 "));
    }
    PUSH_LOG_INFO(line.c_str());
  }
#endif
}

phitos::enums::ESucceed VerifyingCollisionLayerList(const nlohmann::json& json) {
  using phitos::enums::EFound;
  using phitos::enums::ESucceed;
  using opgs16::helper::json::IsJsonKeyExist;

  if (IsJsonKeyExist(json, s_json_list) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file.", s_json_list);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (IsJsonKeyExist(json, s_json_table) == EFound::NotFound) {
    PUSH_LOG_CRITICAL_EXT("Header {} is not found in json file.", s_json_table);
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (json[s_json_list].empty()) {
    PUSH_LOG_CRITICAL_EXT("Collision list must not be empty.");
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (json[s_json_table].empty()) {
    PUSH_LOG_CRITICAL_EXT("Collision checking table must not be empty.");
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  if (json[s_json_list].size() != json[s_json_table].size()) {
    PUSH_LOG_CRITICAL_EXT("Collision layer list and checking list size must be same.");
    PHITOS_UNEXPECTED_BRANCH();
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}
