#include "texture_manager.h"
#include "..\Frame\texture.h"
#include "..\Manager\resource_manager.h"

TextureManager::texture_raw TextureManager::GetTexture(const std::string& name) {
	if (m_container.find(name) == m_container.end()) {
		// Late Initialization
		auto& file_path = ResourceManager::GetInstance().GetTexture2D(name);
		auto texture = std::make_unique<std::remove_pointer_t<texture_raw>>(file_path);
		m_container[name] = std::move(texture);
		return m_container.at(name).get();
	}
	else {
		return m_container.at(name).get();
	}
}

void TextureManager::Release(const std::string& tag) {
	if (DoesAlreadyExist(tag)) {
		m_container.erase(tag);
	}
}

void TextureManager::CheckError() {

}

bool TextureManager::DoesAlreadyExist(const std::string & tag) {
	return m_container.find(tag) != m_container.end();
}
