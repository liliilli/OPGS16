#include "shader_wrapper.h"
#include "shader.h"

void ShaderWrapper::SetShader(opgs16::element::CShaderNew * const shader) {
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
    for (const auto& [name, value] : m_parameters.m_floats)
        m_shader->SetFloat(name, value);
    // Matrix 4x4
    for (const auto& [name, value] : m_parameters.m_mat4s)
        m_shader->SetVecMatrix4f(name, value);
    // Vector2
    for (const auto&[name, value] : m_parameters.m_vec2)
        m_shader->SetVec2f(name, value);
    // Int
    for (const auto&[name, value] : m_parameters.m_ints)
        m_shader->SetInt(name, value);
    // IntPtr
    for (const auto&[name, value] : m_parameters.m_int_ptr)
        m_shader->SetIntPtr(name, value.first, value.second);
}
