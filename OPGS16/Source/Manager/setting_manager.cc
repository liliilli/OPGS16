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
 * @file System/Manager/Private/setting_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-26 Removed redundant move special functions.
 * 2018-03-04 Refactoring.
 * 2018-04-02 std::string to std::wstring in debug log output.
 */

#include "../Public/setting_manager.h"  /*! Header file */

#include <iostream> /*! std::cout */
#include <fstream>  /*! std::ifstream */
#include <sstream>  /*! std::stringstream */
#include <nlohmann/json.hpp> /*! nlohmann::json */
#include "../../Core/Public/logger.h"   /*! ::opgs16::debug::logger */
#include "../../Core/Internal/logger_internal.h"

namespace opgs16 {
namespace manager {
namespace {
constexpr char s_tag_file[] = R"(_Setting/tag.meta)";
constexpr char s_render_layer_file[] = R"(_Setting/render_layer.meta)";
constexpr char s_collision_layer_file[] = R"(_Setting/collision.meta)";

/*!
 * @brief Initialize tag list using file stream.
 * This sequence proceed loading synchronously.
 */
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

/*! Initailize layer list using file stream. */
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

/*! Initialize collision layer list using file stream. */
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
            std::wstring wstr (item.begin(), item.end());
            debug::PushLog(debug::_internal::MsgType::INFO, wstr.c_str() );
        }

        for (const auto& y : check_list) {
            std::wstring line{};
            for (const auto& x : y) {
                line.append((x == true ? L"1 " : L"0 "));
            }
            debug::PushLog(debug::_internal::MsgType::INFO, line.c_str());
        }
#endif
    }
}

} /*! unnamed namespace */

MSettingManager::MSettingManager() {
    InitializeTagList(m_tag_list);
    InitializeRenderLayerList(m_render_layer_list);
    InitializeCollisionLayerList(m_collision_layer_list, m_collision_check_list);
}

std::string MSettingManager::GetTagName(const size_t id) {
    if (id >= m_tag_list.size()) return "";

    return m_tag_list[id];
}

const std::vector<std::string>& MSettingManager::GetTagNameList() const {
    return m_tag_list;
}

} /*! opgs16::manager */
} /*! opgs16 */

