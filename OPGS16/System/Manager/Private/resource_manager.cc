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
 */

#include "../Public/resource_manager.h"   /*! Header file */
#include <fstream>      /*! std::ifstream */
#include <iostream>		/*! std::cerr, std::endl */
#include <sstream>      /*! std::stringstream */
#include <stdexcept>	/*! std::runtime_error */
#include <utility>		/*! std::pair */
#include <vector>		/*! std::vector */

#include "../Internal/resource_internal.h"

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

/*! Get resource type from token. */
EResourceType GetResourceType(const std::string_view& resource_token) {
    using namespace resource::_internal;
    if (resource_token == TEX2D)        return EResourceType::TEXTURE_2D;
    else if (resource_token == SHADE)   return EResourceType::SHADER;
    else if (resource_token == SEBGM)   return EResourceType::SOUND_EFFECT_BGM;
    else if (resource_token == SEEFF)   return EResourceType::SOUND_EFFECT_EFFECT;
    else if (resource_token == FONT)    return EResourceType::FONT;
    else return EResourceType::NOTHING;
}

resource::Texture2D MakeTexture2DContainer(std::stringstream& line_stream,
                                           const std::string& global_path) {
    char type;              line_stream >> type;
    std::string local_path; line_stream >> local_path;
    unsigned x_sep, y_sep;  line_stream >> x_sep >> y_sep;

    return resource::Texture2D{ resource::GetScopeType(type), global_path + local_path, {x_sep, y_sep} };
};

resource::SSound MakeSoundContainer(std::stringstream& line_stream,
                                    const std::string& global_path,
                                    bool is_bgm) {
    char type;              line_stream >> type;
    std::string local_path; line_stream >> local_path;

    return resource::SSound{ resource::GetScopeType(type), global_path + local_path, is_bgm };
};

resource::SShader MakeShaderContainer(std::ifstream& stream,
                                      const std::string& global_path,
                                      const resource::EScopeType scope_type) {
    resource::SShader::shader_list shader_list;

    while (true) {
        std::string token_line;
        std::getline(stream, token_line);
        if (token_line.empty() || (token_line.at(0) != '-'))
            break;
        else {  /*! token_line is not empty and their is something. */
            std::stringstream line_stream{ token_line };
            std::string not_use, shader_token, local_path;
            line_stream >> not_use >> shader_token >> local_path;
            //std::string local_path; line_stream >> local_path;

            shader_list.emplace_back(resource::GetShaderType(shader_token),
                                     global_path + local_path);
        }
    }

    return resource::SShader{ scope_type, shader_list };
};

resource::SFont MakeFontContainer(std::stringstream& line_stream,
                                  const std::string& global_path) {
    char type;          line_stream >> type;
    std::string local_path;   line_stream >> local_path;
    bool is_default;    line_stream >> is_default;
    return resource::SFont{ resource::GetScopeType(type), global_path + local_path, is_default };
};

} /*! unnamed namespace */

namespace manager {

bool ResourceManager::ReadResourceFile(const std::string& path) {
    std::ifstream file_stream{ path, std::ios_base::in };
    file_stream.imbue(std::locale(""));

    if (file_stream.good()) {
        std::string global_path;
        std::string file_line;

        while (std::getline(file_stream, file_line)) {
            if (file_line.empty()) continue;

            switch (auto[symbol, token] = ReadSymbol(file_line); symbol) {
            default: /*! Do nothing */ break;
            case ESymbolType::GLOBAL_PATH: global_path = token; break;
            case ESymbolType::RESOURCE:
                auto token_type = GetResourceType(token);
                ReadResource(file_line, file_stream, global_path, token_type);
                break;
            }
        }

        if (file_stream.eof()) return true;
        else return false;
    }
    else {
        std::cerr << "ERROR::TAG::FILE::CAN::NOT::OPEN\n";
        return false;
    }
}

void ResourceManager::ReadResource(const std::string& token_line,
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
    default: /*! Do nothing */ break;
    case EResourceType::TEXTURE_2D: {    /*! Texture 2D information generating sequence */
        std::string tag;    line_stream >> tag;
        PushTexture2D(tag, MakeTexture2DContainer(line_stream, global_path));
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

void ResourceManager::PushShader(const std::string& name_key,
                                 const resource::SShader& list) {
	if (ExistShaderKey(name_key))
		throw new std::runtime_error("Shader Key duplicated :: " + name_key);

    m_shaders.emplace(name_key, list);
}

void ResourceManager::PushTexture2D(const std::string& name_key,
                                    const resource::Texture2D& container) {
	if (ExistTextureKey(name_key))
		throw new std::runtime_error("Texture Key duplicated :: " + name_key);

	m_textures.emplace(name_key, container);
}

void ResourceManager::PushSound(const std::string& name_key,
                                const resource::SSound& container) {
    if (ExistSoundKey(name_key))
        throw new std::runtime_error("Sound Key duplicated :: " + name_key);

    m_sounds.emplace(name_key, container);

}

void ResourceManager::PushFont(const std::string& name_key,
                               const resource::SFont& container) {
    if (ExistFontKey(name_key))
        throw new std::runtime_error("Font Key duplicated :: " + name_key);

    m_fonts.emplace(name_key, container);
}

const resource::SShader& ResourceManager::GetShader(const std::string& name_key) {
	if (ExistShaderKey(name_key)) {
		return m_shaders.at(name_key);
	}
	else { // Error.
		m_error = ErrorType::FAILED_INITIALIZE_SHADER;
	}
}

const resource::Texture2D& ResourceManager::GetTexture2D(const std::string& name_key) {
	if (ExistTextureKey(name_key)) {
        return m_textures.at(name_key);
	}
	else { /** Error */
		m_error = ErrorType::FAILED_INITIALIZE_TEXTURE2D;
	}
}

const resource::SSound& ResourceManager::GetSound(const std::string& name_key) {
    if (ExistSoundKey(name_key))
        return m_sounds.at(name_key);
}

std::pair<bool, const resource::SFont*> ResourceManager::GetFont(const std::string & name_key) {
    if (ExistFontKey(name_key))
        return { true, &m_fonts.at(name_key) };
    else
        return { false, nullptr };
}

void ResourceManager::CheckError() {
	switch (m_error) {
	case ErrorType::FAILED_INITIALIZE_SHADER:
		std::cerr << "ERROR::FAILED::INITIALIZE::SHADER" << std::endl;
		break;
	case ErrorType::FAILED_INITIALIZE_TEXTURE2D:
		std::cerr << "ERROR::FAILED::INITIALIZE::TEXTURE2D" << std::endl;
		break;
	}
}

} /*! opgs16::manager */
} /*! opgs16 */
