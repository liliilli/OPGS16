#include "shader_manager.h"

ShaderManager::shader_raw ShaderManager::CreateShader
(const std::string& tag, const shader_list& initializer_list) {
	/** Body */
	if (DoesShaderExist(tag)) return GetShaderWithName(tag);

	auto shader = std::make_unique<helper::ShaderNew>();
	for (const auto& pair : initializer_list)
		shader->SetShader(pair.first, pair.second.c_str());
	shader->Link();

	// Bind
	m_shaders[tag] = std::move(shader);
	return m_shaders.at(tag).get();
}

ShaderManager::shader_raw ShaderManager::CreateShader
(const std::string& tag, const shader_vec& list) {
	/** Body */
	if (DoesShaderExist(tag)) return GetShaderWithName(tag);

	auto shader = std::make_unique<helper::ShaderNew>();
	for (const auto& pair : list) {
		shader->SetShader(pair.first, pair.second.c_str());
	}
	shader->Link();

	/** Bind */
	m_shaders[tag] = std::move(shader);
	return m_shaders.at(tag).get();
}

void ShaderManager::CheckError() {
}

void ShaderManager::ReleaseAll() {
}

void ShaderManager::ReleaseShader(const std::string& shader_name) {
}
