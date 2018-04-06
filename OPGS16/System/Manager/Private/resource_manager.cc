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
 * @file System\Manager\resource_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-02 Move ResourceManager class inside opgs16::manager namespace for uniformation.
 *            Refactored and removed member functions are not appropriate for class.
 * 2018-03-03 Move member function to free funtion.
 * 2018-04-05 Get Texture2D to store atlas information in default.
 */

#include "../Public/resource_manager.h"   /*! Header file */

#include <fstream>      /*! std::ifstream */
#include <sstream>      /*! std::stringstream */
#include <stdexcept>	/*! std::runtime_error */
#include <utility>		/*! std::pair */

#include "../../../__ThirdParty/nlohmann/json.hpp"  /*! Third-party json library */
#include "../Internal/resource_internal.h"
#define NOT !

#if defined(_DEBUG)
#include "../../Core/Public/logger.h"
using opgs16::debug::PushLog;
using opgs16::debug::LOG_TYPE_INFO;
using opgs16::debug::LOG_TYPE_WARN;
using opgs16::debug::LOG_TYPE_ERRO;
#endif /*! opgs16::debug::PushLog only on _DEBUG */

namespace opgs16 {

namespace { /*! Helper free function. */

using resource::_internal::EResourceType;
using resource::_internal::ESymbolType;

/*! Read symbol and return with token string. */
std::pair<ESymbolType, std::string> ReadSymbol(const std::string& line_token) {
    ESymbolType symbol{ ESymbolType::NOTHING };

    std::stringstream line_stream{ line_token };
    std::string token; line_stream >> token;

    switch (*token.cbegin()) {
    default: symbol = ESymbolType::RESOURCE; break;
    case '$':   /*! This statements is defualt resource path. */
        symbol = ESymbolType::GLOBAL_PATH;
        line_stream >> token;
        break;
    case '#':   /*! This statements is comment line. */
        symbol = ESymbolType::COMMENT;
        break;
    }

    return { symbol, token };
}

/*!
 * Get resource type from token.
 * @param[in] resource_token
 * @return EResourceType
 */
EResourceType GetResourceType(const std::string_view& resource_token) {
    using namespace resource::_internal;
    if (resource_token == TEX2D)      return EResourceType::TEXTURE_2D;
    if (resource_token == TEX2DATLAS) return EResourceType::TEXTURE_2D_ATLAS;
    if (resource_token == SHADE)    return EResourceType::SHADER;
    if (resource_token == SEBGM)    return EResourceType::SOUND_EFFECT_BGM;
    if (resource_token == SEEFF)    return EResourceType::SOUND_EFFECT_EFFECT;
    if (resource_token == FONT)     return EResourceType::FONT;
    return EResourceType::NOTHING;
}

/*!
 * @brief Make Texture2D without any atlas information.
 * Atlas information will be filled with default information.
 * @param[in] line_stream
 * @param[in] global_path
 */
resource::STexture2DAtlas MakeTexture2DContainerDefault(std::stringstream& line_stream, const std::string& global_path) {
    char type;              line_stream >> type;
    std::string local_path; line_stream >> local_path;
#if defined(_DEBUG)
    std::wstring log{ L"[Texture2D][" };
    log.append(local_path.cbegin(), local_path.cend());     log.append(L"]");
    PushLog(LOG_TYPE_INFO, log.c_str());
#endif /*! Print log in _DEBUG mode */

    resource::STexture2DAtlas atlas_resource;
    atlas_resource.path = global_path + local_path;
    atlas_resource.fragment_number = 1;
    atlas_resource.height = 0;
    atlas_resource.width = 0;

    return atlas_resource;
};

/*!
 * Create atlas information from json stream.
 * @param[in] atlas_json
 * @param[in] list_name
 * @param[in] resource
 * @throw std::runtime_error
 */
template <typename T>
std::vector<T> CreateAtlasInformationList(nlohmann::json& atlas_json, const std::string& list_name,
                                          resource::STexture2DAtlas* resource) {
    if (NOT atlas_json.at(list_name).is_array()) {
        PushLog(LOG_TYPE_ERRO, L"Invalid type specifier, offset_x must be array.");
        throw std::runtime_error("Invalid type specifier, offset_x must be array.");
    }

    const auto list = atlas_json[list_name.c_str()].get<std::vector<T>>();
    if (list.size() != resource->fragment_number) {
        PushLog(LOG_TYPE_ERRO, L"offset_x list size must be same as fragment_number value.");
        throw std::runtime_error("List size is not same as fragment_number.");
    }

    return list;
}

void MakeTexelInformation(resource::STexture2DAtlas& container) {
    const auto texture_width  = container.width;
    const auto texture_height = container.height;
    for (auto i = 0u, size = container.fragment_number; i < size; ++i) {
        auto& texel         = container.texels[i];
        const auto& info    = container.fragment[i];

        const auto start_x  = static_cast<float>(info.offset_x) / texture_width;
        const auto start_y  = 1.f - (static_cast<float>(info.offset_y) / texture_height);
        const auto end_x    = static_cast<float>(info.offset_x + info.width) / texture_width;
        const auto end_y    = 1.f - (static_cast<float>(info.offset_y + info.height) / texture_height);
        texel.right_up[0]   = end_x;
        texel.right_up[1]   = start_y;
        texel.left_down[0]  = start_x;
        texel.left_down[1]  = end_y;
    }
}

/*!
 * @brief Make Texture2D atlas information. if something is wrong, generate runtime_error.
 * @param[in] file
 * @param[in] path
 * @return
 */
resource::STexture2DAtlas MakeTextureAtlasInformation(std::ifstream& file, const std::string& path) {
    nlohmann::json atlas_json;
    file >> atlas_json;

    auto atlas_resource = resource::STexture2DAtlas{};
    atlas_resource.has_atlas       = true;
    atlas_resource.width           = atlas_json.at("texture_width").get<unsigned>();
    atlas_resource.height          = atlas_json.at("texture_height").get<unsigned>();
    atlas_resource.fragment_number = atlas_json.at("item_number").get<unsigned>();
    atlas_resource.fragment.resize(atlas_resource.fragment_number);
    atlas_resource.texels.resize(atlas_resource.fragment_number);
    atlas_resource.path            = path;

    auto offset_x_list  = CreateAtlasInformationList<unsigned>(atlas_json, "offset_x", &atlas_resource);
    auto offset_y_list  = CreateAtlasInformationList<unsigned>(atlas_json, "offset_y", &atlas_resource);
    auto width_list     = CreateAtlasInformationList<unsigned>(atlas_json, "fragment_width", &atlas_resource);
    auto height_list    = CreateAtlasInformationList<unsigned>(atlas_json, "fragment_height", &atlas_resource);
    auto name_list      = CreateAtlasInformationList<std::string>(atlas_json, "name", &atlas_resource);
    for (auto i = 0u; i < atlas_resource.fragment_number; ++i) {
        atlas_resource.fragment[i].offset_x = offset_x_list[i];
        atlas_resource.fragment[i].offset_y = offset_y_list[i];
        atlas_resource.fragment[i].width    = width_list[i];
        atlas_resource.fragment[i].height   = height_list[i];
        atlas_resource.fragment[i].name     = name_list[i];
    }
    MakeTexelInformation(atlas_resource);
    return atlas_resource;
}

/*!
 * @brief Make Texture2D with specific atlas information
 * @param[in] line_stream
 * @param[in] global_path
 * @throw std::runtime_error
 * @return
 */
resource::STexture2DAtlas MakeTexture2DAtlasContainer(std::stringstream& line_stream, const std::string& global_path) {
    char type; line_stream >> type;
    std::string local_texture_path; line_stream >> local_texture_path;
    std::string local_atlas_path; line_stream >> local_atlas_path;
#if defined(_DEBUG)
    {
        std::wstring log; log.resize(150); log.append(L"[Texture2DAtlas][");
        log.append(local_texture_path.cbegin(), local_texture_path.cend()); log.append(L"][");
        log.append(local_atlas_path.cbegin(), local_atlas_path.cend()); log.append(L"]");
        PushLog(LOG_TYPE_INFO, log.c_str());
    }
#endif /*! Print local pathes */

    std::string atlas_path = global_path + local_atlas_path;
    std::ifstream file{ atlas_path.c_str(), std::ios_base::in };
    if (file.bad()) {
        std::wstring log; log.resize(150); log.append(L"Failed to read ");
        log.append(atlas_path.cbegin(), atlas_path.cend());
        PushLog(LOG_TYPE_ERRO, log.c_str());
        throw std::runtime_error("Failed to read resource file.");
    }

    return MakeTextureAtlasInformation(file, global_path + local_texture_path);
}

/*!
 * @brief Make Sound as background music
 * @param[in] line_stream
 * @param[in] global_path
 * @param[in] is_bgm
 * @return
 */
resource::SSound MakeSoundContainer(std::stringstream& line_stream, const std::string& global_path,
                                    bool is_bgm) {
    char type;              line_stream >> type;
    std::string local_path; line_stream >> local_path;
#if defined(_DEBUG)
    {
        std::wstring log{ L"[Sound][" };
        log.append(local_path.cbegin(), local_path.cend());     log.append(L"]");
        PushLog(LOG_TYPE_INFO, log.c_str());
    }
#endif
    return resource::SSound{ resource::GetScopeType(type), global_path + local_path, is_bgm };
};

/*!
 * @brief Make shader container
 * @param[in] stream
 * @param[in] global_path
 * @param[in] scope_type
 * @return
 */
resource::SShader MakeShaderContainer(std::ifstream& stream, const std::string& global_path,
                                      const resource::EScopeType scope_type) {
    resource::SShader::shader_list shader_list;

    while (true) {
        std::string token_line;
        std::getline(stream, token_line);
        if (token_line.empty() || (token_line.at(0) != '-')) break;
        else {  /*! token_line is not empty and their is something. */
            std::stringstream line_stream{ token_line };
            std::string not_use, shader_token, local_path;
            line_stream >> not_use >> shader_token >> local_path;

            shader_list.emplace_back(resource::GetShaderType(shader_token), global_path + local_path);
        }
    }
#if defined(_DEBUG)
    {
        PushLog(LOG_TYPE_INFO, L"[SHADER]");
        for (const auto& shader : shader_list) {
            std::wstring log{ L"[SHADER_ELEMENT][" };
            log.append(shader.second.cbegin(), shader.second.cend()); log.append( L"]" );
            PushLog(LOG_TYPE_INFO, log.c_str());
        }
    }
#endif
    return resource::SShader{ scope_type, shader_list };
};

/*!
 * @brief Make font container
 * @param[in] line_stream
 * @param[in] global_path
 * @return
 */
resource::SFont MakeFontContainer(std::stringstream& line_stream, const std::string& global_path) {
    char type;              line_stream >> type;
    std::string local_path; line_stream >> local_path;
    bool is_default;        line_stream >> is_default;
#if defined(_DEBUG)
    {
        std::wstring log{ L"[FontGlyph][" };
        log.append(local_path.cbegin(), local_path.cend());     log.append(L"][");
        log.append(is_default ? L"DEFAULT" : L"NOT_DEFAULT");   log.append(L"]");
        PushLog(LOG_TYPE_INFO, log.c_str());
    }
#endif
    return resource::SFont{ resource::GetScopeType(type), global_path + local_path, is_default };
};

} /*! unnamed namespace */

namespace manager {

bool MResourceManager::ReadResourceFile(const wchar_t* path) {
    std::ifstream file_stream{ path, std::ios_base::in };
    file_stream.imbue(std::locale(""));

    if (file_stream.good()) {
#if defined(_DEBUG)
        PushLog(LOG_TYPE_WARN, (std::wstring{ L"Opened resource meta file : " } + path).c_str());
#endif
        std::string global_path, file_line;
        while (std::getline(file_stream, file_line)) {
            if (file_line.empty()) continue;

            switch (auto[symbol, token] = ReadSymbol(file_line); symbol) {
            default: break;
            case ESymbolType::GLOBAL_PATH:
                global_path = token;
#if defined(_DEBUG)
                PushLog(LOG_TYPE_WARN, std::wstring(global_path.begin(), global_path.end()).c_str());
#endif
                break;
            case ESymbolType::RESOURCE:
                ReadResource(file_line, file_stream, global_path, GetResourceType(token));
                break;
            }
        }

        return file_stream.eof();
    }
    else {
#if defined(_DEBUG)
        PushLog(LOG_TYPE_ERRO, (std::wstring{ L"Cannot open the file : " } + path).c_str());
#endif
        return false;
    }
}

void MResourceManager::ReadResource(const std::string& token_line,
                                   std::ifstream& stream,
                                   const std::string& global_path,
                                   EResourceType type) {
    std::stringstream line_stream{ token_line };
    /*! Just drop first token */
    {
        std::string __;
        line_stream >> __;
    }

    switch (type) {
    default: break;
    case EResourceType::TEXTURE_2D: {    /*! Texture 2D information generating sequence */
        std::string tag;    line_stream >> tag;
        PushTexture2D(tag, MakeTexture2DContainerDefault(line_stream, global_path));
    }   break;
    case EResourceType::TEXTURE_2D_ATLAS: {
        std::string tag; line_stream >> tag;
        PushTexture2D(tag, MakeTexture2DAtlasContainer(line_stream, global_path));
    }   break;
    case EResourceType::SHADER: {
        std::string tag;    line_stream >> tag;
        char type;          line_stream >> type;
        PushShader(tag, MakeShaderContainer(stream, global_path, resource::GetScopeType(type)));
    }   break;
    case EResourceType::SOUND_EFFECT_BGM: {
        std::string tag;    line_stream >> tag;
        PushSound(tag, MakeSoundContainer(line_stream, global_path, true));
    }   break;
    case EResourceType::SOUND_EFFECT_EFFECT: {
        std::string tag;    line_stream >> tag;
        PushSound(tag, MakeSoundContainer(line_stream, global_path, false));
    }   break;
    case EResourceType::FONT: {
        std::string tag;    line_stream >> tag;
        PushFont(tag, MakeFontContainer(line_stream, global_path));
    }   break;
    }
}

void MResourceManager::PushShader(const std::string& name_key, const resource::SShader& list) {
	if (ExistKey(m_shaders, name_key)) {
#if defined(_DEBUG)
        PushLog(LOG_TYPE_ERRO, L"Shader key duplicated");
#endif
		throw std::runtime_error("Shader Key duplicated :: " + name_key);
	}

    m_shaders.emplace(name_key, list);
}

void MResourceManager::PushTexture2D(const std::string& name_key, const resource::STexture2DAtlas& container) {
	if (ExistKey(m_textures, name_key))
		throw std::runtime_error("Texture Key duplicated :: " + name_key);
	m_textures.emplace(name_key, container);
}

void MResourceManager::PushSound(const std::string& name_key, const resource::SSound& container) {
    if (ExistKey(m_sounds, name_key))
        throw std::runtime_error("Sound Key duplicated :: " + name_key);
    m_sounds.emplace(name_key, container);
}

void MResourceManager::PushFont(const std::string& name_key, const resource::SFont& container) {
    if (ExistKey(m_fonts, name_key))
        throw std::runtime_error("Font Key duplicated :: " + name_key);
    m_fonts.emplace(name_key, container);
}

const resource::SShader& MResourceManager::GetShader(const std::string& name_key) {
	if (NOT ExistKey(m_shaders, name_key)) {
#if defined(_DEBUG)
        std::wstring log{ L"Does not found proper shader, " };
        log.append(std::cbegin(name_key), std::cend(name_key));
        PushLog(LOG_TYPE_ERRO, log.c_str());
        PushLog(LOG_TYPE_ERRO, L"Return dummy shader...");
#endif /*! Log error message in Debug mode */
        /*! Temporary */
        throw std::runtime_error("GetShader error");
	}

    return m_shaders[name_key];
}

const resource::STexture2DAtlas& MResourceManager::GetTexture2D(const std::string& name_key) {
	if (NOT ExistKey(m_textures, name_key)) {
#if defined(_DEBUG)
        std::wstring log{ L"Does not found proper texture2D, " };
        log.append(std::cbegin(name_key), std::cend(name_key));
        PushLog(LOG_TYPE_ERRO, log.c_str());
#endif /*! Log error message in Debug mode */
        /*! Temporary */
        throw std::runtime_error("GetTexture2D error");
	}

    return m_textures[name_key];
}

const resource::SSound& MResourceManager::GetSound(const std::string& name_key) {
    if (NOT ExistKey(m_sounds, name_key)) {
#if defined(_DEBUG)
        std::wstring log{ L"Does not found proper sound, " };
        log.append(std::cbegin(name_key), std::cend(name_key));
        PushLog(LOG_TYPE_ERRO, log.c_str());
#endif /*! Log error message in Debug mode */
        /* Temporary */
        throw std::runtime_error("GetSound error");
    }

    return m_sounds[name_key];
}

std::pair<bool, const resource::SFont*> MResourceManager::GetFont(const std::string & name_key) {
    if (ExistKey(m_fonts, name_key))
        return { true, &m_fonts[name_key] };
    else
        return { false, nullptr };
}

} /*! opgs16::manager */
} /*! opgs16 */
