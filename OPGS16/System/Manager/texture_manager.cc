#include "texture_manager.h"
#include "..\Manager\resource_manager.h"

namespace {
using texture_raw = TextureManager::texture_raw;
}

texture_raw TextureManager::Create(const std::string& tag,
								   const texture::Texture2D& texture) {
	return texture_raw();
}

texture_raw TextureManager::Create(const std::string& name) {
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

texture_raw TextureManager::TempCreateImage(const std::string& tag,
											const std::string& image_path,
											const unsigned mode) {
	if (!IsAlreadyExist(tag)) {
		auto texture = std::make_unique<std::remove_pointer_t<texture_raw>>(image_path, mode);
		m_container[tag] = std::move(texture);
		return m_container.at(tag).get();
	}
	else {
		m_error = ErrorType::ALREADY_HAS_INSTANCE;
		return nullptr;
	}
}

texture_raw TextureManager::GetTexture(const std::string& tag) {
	return texture_raw();
}

void TextureManager::Release(const std::string& tag) {
}

void TextureManager::CheckError()
{
}
