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
 */

#include "../Public/setting_manager.h"  /*! Header file */

#include <iostream> /*! std::cout */
#include <fstream>  /*! std::ifstream */
#include <sstream>  /*! std::stringstream */

namespace opgs16 {
namespace manager {

namespace {
constexpr char s_tag_file[] = R"(_Setting/tag.meta)";
constexpr char s_layer_file[] = R"(_Setting/layer.meta)";
} /*! unnamed namespace */

SettingManager::SettingManager() {
    InitializeTagList();
    InitializeLayerList();
}

void SettingManager::InitializeTagList() {
    std::ifstream in_file_stream(s_tag_file, std::ios_base::in);
    in_file_stream.imbue(std::locale(""));

    if (in_file_stream.good()) {
        std::string tag;
        while (in_file_stream >> tag) {
            if (!tag.empty()) m_tag_list.emplace_back(tag);
        }

        // DEBUG
        for (size_t i = 0u; i < m_tag_list.size(); ++i) {
            std::cout << i << " : " << m_tag_list[i] << "\n";
        }
    }
    else {
        std::cerr << "ERROR::TAG::FILE::CAN::NOT::OPEN\n";
    }
}

void SettingManager::InitializeLayerList() {
    std::cout << "LAYER FEATURE IS NOT IMPLEMENTED, COMMING SOON\n";
}

std::string SettingManager::GetTagName(const size_t id) {
    if (id >= m_tag_list.size()) return "";

    return m_tag_list[id];
}

const std::vector<std::string>& SettingManager::GetTagNameList() const {
    return m_tag_list;
}

} /*! opgs16::manager */
} /*! opgs16 */

