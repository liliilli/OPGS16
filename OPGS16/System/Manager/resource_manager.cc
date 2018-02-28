#include "resource_manager.h"   /*! Header file */
#include <fstream>      /*! std::ifstream */
#include <iostream>		/** std::cerr, std::endl */
#include <sstream>      /*! std::stringstream */
#include <stdexcept>	/** std::runtime_error */
#include <vector>		/** std::vector */

enum class ResourceManager::ResourceType {
    NOTHING,                    /*! Error type nothing found from token. */
    TEXTURE_2D,                 /*! Texture 2d */
    SHADER,                     /*! Shader program */
    SOUND_EFFECT_BGM,           /*! SE background music */
    SOUND_EFFECT_EFFECT,        /*! SE effect sound */
};

bool ResourceManager::LoadResource(const std::string& path) {
    std::ifstream file_stream{ path, std::ios_base::in };
    file_stream.imbue(std::locale(""));

    if (file_stream.good()) {
        std::string global_path;
        std::string file_line;

        while (std::getline(file_stream, file_line)) {
            if (file_line.empty()) continue;

            switch (auto[symbol, token] = ReadSymbol(file_line); symbol) {
            default: /*! Do nothing */ break;
            case SymbolType::GLOBAL_PATH: global_path = token; break;
            case SymbolType::RESOURCE:
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

void ResourceManager::InsertSampleResources() {
	//using namespace std::string_literals;
	//PushShader("gQuad", {
	//	{shader_type::VS, "Shaders/Global/image.vert"s},
	//	{shader_type::FS, "Shaders/Global/image.frag"s}
	//	}
	//);

	//PushShader("gCommonFont", {
	//	{shader_type::VS, "Shaders/Global/font.vert"s},
	//	{shader_type::FS, "Shaders/Global/font.frag"s}
	//	}
	//);

	//PushShader("ppQuad", {
	//	{ shader_type::VS, "Shaders/Global/quad.vert"s },
	//	{ shader_type::FS, "Shaders/Global/quad.frag"s }
	//	}
	//);

	//PushShader("ppConvex", {
	//	{ shader_type::VS, "Shaders/Global/quad.vert"s },
	//	{ shader_type::FS, "Shaders/Global/convex.frag"s }
	//	}
	//);

	//PushShader("ppGray", {
	//	{ shader_type::VS, "Shaders/Global/quad.vert"s },
	//	{ shader_type::FS, "Shaders/pp/gray.frag"s }
	//	}
	//);

	//PushShader("ppSineWave", {
	//	{ shader_type::VS, "Shaders/Global/quad.vert"s },
	//	{ shader_type::FS, "Shaders/Global/sinewave.frag"s }
	//	}
	//);

	//PushTexture2D("Test", "Resources/test_2.png");
}

std::pair<ResourceManager::SymbolType, std::string>
ResourceManager::ReadSymbol(const std::string& line_token) {
    SymbolType symbol{ SymbolType::NOTHING };
    std::stringstream line_stream{ line_token };

    std::string token; line_stream >> token;
    switch (*token.cbegin()) {
    default:
        symbol = SymbolType::RESOURCE;      /*! This symbol is for Resouce token. */
        break;
    case '$':   /*! This statements is defualt resource path. */
        symbol = SymbolType::GLOBAL_PATH;   /*! This symbol is for global path token. */
        line_stream >> token;
        break;
    case '#':   /*! This statements is comment line. */
        symbol = SymbolType::COMMENT;       /*! This symbol is for statement token. */
        break;
    }

    return { symbol, token };
}

ResourceManager::ResourceType
ResourceManager::GetResourceType(const std::string & resource_token) {
    if (resource_token == TEX2D)        return ResourceType::TEXTURE_2D;
    else if (resource_token == SHADE)   return ResourceType::SHADER;
    else if (resource_token == SEBGM)   return ResourceType::SOUND_EFFECT_BGM;
    else return ResourceType::NOTHING;
}

void ResourceManager::ReadResource(const std::string& token_line,
                                   std::ifstream& stream,
                                   const std::string& global_path,
                                   ResourceManager::ResourceType type) {
    std::string __;

    switch (std::stringstream line_stream{ token_line }; type) {
    default: /*! Do nothing */ break;
    case ResourceType::TEXTURE_2D: {    /*! Texture 2D information generating sequence */
        line_stream >> __;

        /*! Read information from stream */
        std::string tag;        line_stream >> tag;
        char type;              line_stream >> type;
        std::string local_path; line_stream >> local_path;
        unsigned x_sep, y_sep;  line_stream >> x_sep >> y_sep;

        /*! Insert */
        PushTexture2D(tag,
                      resource::Texture2D{ resource::GetScopeType(type),
                                           global_path + local_path,
                                           {x_sep, y_sep}
                      });
    }   break;
    case ResourceType::SHADER: {
        line_stream >> __;

        std::string tag;    line_stream >> tag;
        char type;          line_stream >> type;
        auto shader_list{ ReadShaderPath(stream, global_path) };/*! C++17 can accepts this. */
        /*! Insert */
        PushShader(tag, shader_list);
    }   break;
    case ResourceType::SOUND_EFFECT_BGM: {
        line_stream >> __;

        std::string tag;    line_stream >> tag;
        char type;          line_stream >> type;
        std::string path;   line_stream >> path;
        path = global_path + path;
        /*! Insert */
        PushSound(tag, path);
    }   break;
    }
}

void ResourceManager::ReadTexture2D(std::ifstream& line_stream,
                                    const std::string& global_path) {
}

std::vector<ResourceManager::shader_pair>
ResourceManager::ReadShaderPath(std::ifstream& stream,
                                const std::string& global_path) {
    std::vector<ResourceManager::shader_pair> shader_list;

    while (true) {
        std::string token_line;
        std::getline(stream, token_line);
        if (token_line.empty() || (token_line.at(0) != '-'))
            break;
        else {  /*! token_line is not empty and their is something. */
            std::stringstream line_stream{ token_line };
            /*! __ is used just for neglecting - (hypon character) */
            std::string __; std::string token; line_stream >> __ >> token;

            shader_type type;
            if (token == "VS")          type = shader_type::VS;
            else if (token == "GS")     type = shader_type::GS;
            else if (token == "TCS")    type = shader_type::TCS;
            else if (token == "TES")    type = shader_type::TES;
            else if (token == "FS")     type = shader_type::FS;

            line_stream >> token;
            std::string file_path = global_path + token;

            shader_list.emplace_back(std::make_pair(type, file_path));
        }
    }

    return shader_list;
}

void ResourceManager::PushShader(const std::string& name_key, const shader_list& list) {
	if (ExistShaderKey(name_key))
		throw new std::runtime_error("Shader Key duplicated :: " + name_key);

	std::vector<shader_pair>&& shader_list{};
	for (const shader_pair& item : list) { shader_list.push_back(item); }
	m_shaders[name_key] = std::move(shader_list);
}

ResourceManager::shader_list& ResourceManager::GetShader(const std::string& name_key) {
	if (ExistShaderKey(name_key)) {
		return m_shaders.at(name_key);
	}
	else { // Error.
		m_error = ErrorType::FAILED_INITIALIZE_SHADER;
	}
}

void ResourceManager::PushTexture2D(const std::string& name_key,
                                    const resource::Texture2D& container) {
	if (ExistTextureKey(name_key))
		throw new std::runtime_error("Texture Key duplicated :: " + name_key);

	m_textures.emplace(std::make_pair(name_key, container));
}

void ResourceManager::PushSound(const std::string& name_key, const std::string& path) {
    if (ExistSoundKey(name_key))
        throw new std::runtime_error("Sound Key duplicated :: " + name_key);

    m_sounds.emplace(std::make_pair(name_key, path));

}

const resource::Texture2D& ResourceManager::GetTexture2D(const std::string& name_key) {
	if (ExistTextureKey(name_key)) {
        return m_textures.at(name_key);
	}
	else { /** Error */
		m_error = ErrorType::FAILED_INITIALIZE_TEXTURE2D;
	}
}

const std::string& ResourceManager::GetSound(const std::string& name_key) {
    if (ExistSoundKey(name_key))
        return m_sounds.at(name_key);
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

ResourceManager::ResourceManager() = default;