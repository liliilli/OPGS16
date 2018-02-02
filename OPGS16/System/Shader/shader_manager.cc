#include "shader_manager.h"

std::shared_ptr<helper::ShaderNew> ShaderManager::CreateShader(const std::string&& tag,
	std::initializer_list<std::pair<Type, const std::string&&>> initializer_list) {
	/** Body */
	if (m_shaders.find(tag) != m_shaders.end()) return GetShaderWithName(std::move(tag));

	auto shader = std::make_shared<helper::ShaderNew>();
	for (const auto& pair : initializer_list)
		shader->SetShader(pair.first, pair.second.c_str());

	shader->Link();

	// Bind
	m_shaders[tag] = shader;
	return shader;
}

void ShaderManager::DrawWithShader(std::string && name) {
}

void ShaderManager::BindObjectToShader(std::string && name, const Object & object) {
}

bool ShaderManager::Fail() {
	return false;
}

void ShaderManager::CleanAll() {
}

void ShaderManager::CleanWithTag() {
}
