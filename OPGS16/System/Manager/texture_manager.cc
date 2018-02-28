#include "texture_manager.h"
#include "..\Frame\texture.h"
#include "..\Manager\resource_manager.h"

TextureManager::texture_raw TextureManager::GetTexture(const std::string& name) {
	if (!DoesAlreadyExist(name)) {
		const auto& file_path = ResourceManager::GetInstance().GetTexture2D(name);

        /*! _ is unused. */
        auto [it, _] = m_container.emplace(
            std::make_pair(name, std::make_unique<texture::Texture2D>(file_path)));
		return it->second.get();
	}
	else
        return m_container.at(name).get();
}

void TextureManager::Release(const std::string& tag) {
	if (DoesAlreadyExist(tag)) {
		m_container.erase(tag);
	}
}

void TextureManager::CheckError() {

}

void TextureManager::Clear() {
    m_container.clear();
}

bool TextureManager::DoesAlreadyExist(const std::string& tag) {
	return m_container.find(tag) != m_container.end();
}
