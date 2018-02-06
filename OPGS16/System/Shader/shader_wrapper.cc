#include "shader_wrapper.h"

void ShaderWrapper::BindShader(helper::ShaderNew* const shader) {
	m_shader = shader;
	m_is_useable = true;
}
