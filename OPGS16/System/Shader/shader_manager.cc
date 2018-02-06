#include "shader_manager.h"

ShaderManager::shader_raw ShaderManager::CreateShader
(const std::string&& tag, shader_list initializer_list) {
	/** Body */
	if (m_shaders.find(tag) != m_shaders.end()) return GetShaderWithName(std::move(tag));

	auto shader = std::make_unique<helper::ShaderNew>();
	for (const auto& pair : initializer_list)
		shader->SetShader(pair.first, pair.second.c_str());
	shader->Link();

	// Bind
	m_shaders[tag] = std::move(shader);
	return m_shaders.at(tag).get();
}

ShaderManager::shader_raw ShaderManager::CreateShader
(const std::string& tag, const container& list) {
	/** Body */
	if (m_shaders.find(tag) != m_shaders.end()) return GetShaderWithName(std::move(tag));

	auto shader = std::make_unique<helper::ShaderNew>();
	for (const auto& pair : list) {
		shader->SetShader(pair.first, pair.second.c_str());
	}
	shader->Link();

	/** Bind */
	m_shaders[tag] = std::move(shader);
	return m_shaders.at(tag).get();
}

void ShaderManager::BindObjectToShader(std::string && name, const Object & object) {
}

void ShaderManager::CheckError() {
}

void ShaderManager::CleanAll() {
}

void ShaderManager::CleanWithTag() {
}
