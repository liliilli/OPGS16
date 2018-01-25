#include "shader_manager.h"

helper::ShaderNew& ShaderManager::GetShaderWithName(std::string&& name) {
	return *m_shaders[name];
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
