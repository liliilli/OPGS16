#include "resource_manager.h"
#include <initializer_list>
#include <fstream>      /*! std::ifstream */
#include <iostream>		/** std::cerr, std::endl */
#include <sstream>      /*! std::stringstream */
#include <stdexcept>	/** std::runtime_error */
#include <vector>		/** std::vector */

ResourceManager::ResourceManager() {}

void ResourceManager::LoadResource(const std::string& path) {
	using namespace std::string_literals;
	PushShader("gQuad", {
		{shader_type::VS, "Shaders/Global/image.vert"s},
		{shader_type::FS, "Shaders/Global/image.frag"s}
		}
	);

	PushShader("gCommonFont", {
		{shader_type::VS, "Shaders/Global/font.vert"s},
		{shader_type::FS, "Shaders/Global/font.frag"s}
		}
	);

	PushShader("ppQuad", {
		{ shader_type::VS, "Shaders/Global/quad.vert"s },
		{ shader_type::FS, "Shaders/Global/quad.frag"s }
		}
	);

	PushShader("ppConvex", {
		{ shader_type::VS, "Shaders/Global/quad.vert"s },
		{ shader_type::FS, "Shaders/Global/convex.frag"s }
		}
	);

	PushShader("ppGray", {
		{ shader_type::VS, "Shaders/Global/quad.vert"s },
		{ shader_type::FS, "Shaders/pp/gray.frag"s }
		}
	);

	PushShader("ppSineWave", {
		{ shader_type::VS, "Shaders/Global/quad.vert"s },
		{ shader_type::FS, "Shaders/Global/sinewave.frag"s }
		}
	);

	PushTexture2D("Test", "Resources/test_2.png");

    /*! Read */
    std::ifstream file{ path, std::ios_base::in };
    file.imbue(std::locale(""));

    if (file.good()) {
        std::string word;
        while (file >> word) {
            if (word == "TEX2D") /*! If resource to read is texture 2d */ {
                std::string item_tag;
                file >> item_tag;
                std::string item_path;
                file >> item_path;
                /*! Instantiate Texture 2D */
                PushTexture2D(item_tag, item_path);
            }
        }
    }
    else {
        std::cerr << "ERROR::TAG::FILE::CAN::NOT::OPEN\n";
    }
}

void ResourceManager::PushShader(const std::string& name_key, const shader_list& list) {
	if (ExistShaderKey(name_key))
		throw new std::runtime_error("Shader Key duplicated :: " + name_key);

	std::vector<shader_pair>&& shader_list{};
	for (const shader_pair& item : list) { shader_list.push_back(item); }
	m_shaders[name_key] = std::move(shader_list);
}

ResourceManager::shader_container& ResourceManager::GetShader(const std::string& name_key) {
	if (ExistShaderKey(name_key)) {
		return m_shaders.at(name_key);
	}
	else { // Error.
		m_error = ErrorType::FAILED_INITIALIZE_SHADER;
	}
}

void ResourceManager::PushTexture2D(const std::string& name_key, const std::string& path) {
	if (ExistTextureKey(name_key))
		throw new std::runtime_error("Texture Key duplicated :: " + name_key);

	m_textures[name_key] = path;
}

const std::string& ResourceManager::GetTexture2D(const std::string& name_key) {
	if (ExistTextureKey(name_key)) {
		return m_textures.at(name_key);
	}
	else { /** Error */
		m_error = ErrorType::FAILED_INITIALIZE_TEXTURE2D;
	}
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
