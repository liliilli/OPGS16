#include "shader_wrapper.h"
#include "shader.h"

void ShaderWrapper::SetShader(helper::ShaderNew * const shader) {
    m_shader = shader;
    m_is_useable = true;
}

void ShaderWrapper::UseShader() {
    if (m_is_useable) {
        m_shader->Use();
        RefreshUniformValues();
    }
}

void ShaderWrapper::RefreshUniformValues() {
    // Float
    for (const auto& item : m_parameters.m_floats)
        m_shader->SetFloat(item.first, item.second);
    // Matrix 4x4
    for (const auto& item : m_parameters.m_mat4s)
        m_shader->SetVecMatrix4f(item.first, item.second);
}
