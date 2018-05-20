
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file System/Manager/Private/setting_manager.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-26 Removed redundant move special functions.
/// 2018-03-04 Refactoring.
/// 2018-04-02 std::string to std::wstring in debug log output.
/// 2018-05-20 Detach singleton pattern into namespace.
///

#include <Manager\setting_manager.h>  /// Header file

#include <fstream>  
#include <string>   
#include <vector>   

#include <nlohmann\json.hpp> /// nlohmann::json library

/// Import logger
#include <Headers\import_logger.h>
/// Expanded assertion
#include <Helper\assert.h>
/// ::opgs16::manager::_internal flags
#include <Manager\Internal\flag.h>
/// ::opgs16::debug error messages.
#include <Manager\Internal\error_message.h>

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Member container
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// This namespace is integrity check variable container for 
/// checking runtime caveats of source code.
///
namespace {
using opgs16::manager::_internal::EInitiated;

EInitiated m_initiated = EInitiated::NotInitiated;
  
} /// unnamed namespace

///
/// This namespace stores variables or constexpr variables to be used
/// functions.
/// 
namespace {

constexpr char s_tag_file[] = R"(_Setting/tag.meta)";
constexpr char s_render_layer_file[] = R"(_Setting/render_layer.meta)";
constexpr char s_collision_layer_file[] = R"(_Setting/collision.meta)";

/// Tag list used in Application.
std::vector<std::string> m_tag_list;    
/// Render layer list
std::vector<std::string> m_render_layer_list;
/// Collision Layer list.
std::vector<std::string> m_collision_layer_list;

std::vector<std::vector<bool>> m_collision_check_list;

} /// unnamed namespace

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// Implementation part
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

///
/// @brief Initialize tag list using file stream.
/// This sequence proceed loading synchronously.
///
void InitializeTagList(std::vector<std::string>& tag_list) {
  std::ifstream in_file_stream(s_tag_file, std::ios_base::in);
  in_file_stream.imbue(std::locale(""));

  if (in_file_stream.good()) {
    std::string tag;
    while (in_file_stream >> tag) {
      if (!tag.empty()) tag_list.emplace_back(tag);
    }

#ifdef _DEBUG
    for (auto i = 0u; i < tag_list.size(); ++i) {
      std::cout << i << " : " << tag_list[i] << "\n";
    }
#endif
  }
  else {
    std::cerr << "ERROR::TAG::FILE::CAN::NOT::OPEN\n";
  }
}

///
/// @brief 
/// Initailize layer list using file stream.
///
void InitializeRenderLayerList(std::vector<std::string>& layer_list) {
  std::ifstream file_stream{ s_render_layer_file, std::ios_base::in };
  file_stream.imbue(std::locale(""));

  if (file_stream.good()) {
    std::string layer_name;
    while (file_stream >> layer_name) {
      if (!layer_name.empty()) layer_list.emplace_back(layer_name);
    }

#ifdef _DEBUG
    for (auto i = 0u; i < layer_list.size(); ++i) {
      std::cout << i << " : " << layer_list[i] << "\n";
    }
#endif
  }
}

///
/// @brief 
/// Initialize collision layer list using file stream.
///
void InitializeCollisionLayerList(std::vector<std::string>& layer_list,
  std::vector<std::vector<bool>>& check_list) {
  std::ifstream file_stream{ s_collision_layer_file, std::ios_base::in };
  file_stream.imbue(std::locale(""));
  // COLLISION CHECK TABLE
  // [ ][0][1][2][3] // [0][ ][x][x][ ] // [1]___[x][x][x] // [2]______[ ][ ] // [3]_________[ ]
  if (file_stream.good()) {
    nlohmann::json json; file_stream >> json;

    for (const auto& element : json["list"])
      layer_list.emplace_back(element.get<std::string>());

  /*! Make table */
    const auto size = layer_list.size();
    check_list.resize(size);
    for (auto i = size; i > 0; --i) check_list[size - i].resize(i);

    /*! Set collision table */
    {
      size_t i = 0;
      for (const auto& element : json["table"]) {
        auto list = element.get<std::vector<int>>();
        if (list.empty()) goto l_loop_end;

        for (const auto& index : list) {
          check_list[i][size - index - 1] = true;
        }
l_loop_end:
        ++i;
      }
    }

#if defined(_DEBUG)
    for (const auto& item : layer_list) {
      std::wstring wstr(item.begin(), item.end());
      PUSH_LOG_INFO(wstr.c_str());
    }

    for (const auto& y : check_list) {
      std::wstring line{};
      for (const auto& x : y) {
        line.append((x == true ? L"1 " : L"0 "));
      }
      PUSH_LOG_INFO(line.c_str());
    }
#endif
  }
}

namespace opgs16::manager::setting {

void Initiate() {
  // Integrity check.
  NEU_ASSERT(m_initiated == EInitiated::NotInitiated,
      debug::err_setting_duplicated_init);

  m_initiated = EInitiated::Initiated;

  InitializeTagList(m_tag_list);
  InitializeRenderLayerList(m_render_layer_list);
  InitializeCollisionLayerList(m_collision_layer_list, m_collision_check_list);
}

const std::string& GetTagName(size_t i) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  NEU_ASSERT(i < m_tag_list.size(), debug::err_setting_tag_oob);

  return m_tag_list[i];
}

const std::string& GetRenderingLayerName(size_t i) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  NEU_ASSERT(i < m_render_layer_list.size(), debug::err_setting_rendering_oob);

  return m_render_layer_list[i];
}

const TagList& GetTagNameList() {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_tag_list;
}

const RenderingLayerList& GetRenderingLayerNameList() {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_render_layer_list;
}

size_t GetRenderingLayerNameListSize() {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);

  return m_render_layer_list.size();
}

const std::string& GetCollisionLayerName(size_t i) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  NEU_ASSERT(i < m_collision_layer_list.size(),
      debug::err_setting_collision_oob);

  return m_collision_layer_list[i];
}

const CollisionLayerLIst& GetCollisionLayerList() {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  
  return m_collision_layer_list;
}

bool CollisionLayerCheck(unsigned lhs, unsigned rhs) {
  NEU_ASSERT(m_initiated == EInitiated::Initiated,
      debug::err_setting_not_initiated);
  NEU_ASSERT(lhs < m_collision_check_list.size(),
      debug::err_setting_collision_check_oob);
  NEU_ASSERT(rhs < m_collision_check_list.size(),
      debug::err_setting_collision_check_oob);

  const auto size = m_collision_check_list.size();
  if (lhs > rhs)  
    return m_collision_check_list[rhs][size - lhs - 1];
  else
    return m_collision_check_list[lhs][size - rhs - 1];
}

} /// ::opgs16::manager::setting

