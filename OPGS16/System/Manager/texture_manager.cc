#include "texture_manager.h"
#include "..\Frame\texture.h"
#include "..\Manager\resource_manager.h"

namespace { using texture_raw = TextureManager::texture_raw; }

texture_raw TextureManager::GetTexture(const std::string& name) {
	if (m_container.find(name) == m_container.end()) {
		// Late Initialization
		auto& file_path = ResourceManager::GetInstance().GetTexture2D(name);
		auto texture = std::make_unique<std::remove_pointer_t<texture_raw>>(file_path, GL_RGBA);
		m_container[name] = std::move(texture);
		return m_container.at(name).get();
	}
	else {
		m_error = ErrorType::ALREADY_HAS_INSTANCE;
		return nullptr;
	}
}

void TextureManager::Release(const std::string& tag) {
	if (IsAlreadyExist(tag)) {
		m_container.erase(tag);
	}
}

void TextureManager::CheckError() {

}

bool TextureManager::IsAlreadyExist(const std::string & tag) {
	return m_container.find(tag) != m_container.end();
}
