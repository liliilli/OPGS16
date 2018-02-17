#include "setting_manager.h"

#include <iostream> /*! std::cout */
#include <fstream>  /*! std::ifstream */
#include <sstream>  /*! std::stringstream */

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
