
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Shader/shader_wrapper.cc
 * @brief Definition file of ./shader_wrapper.h
 * @log
 * 2018-04-17 Move definition files to ::opgs16::element.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Shader\shader_wrapper.h>  /// Header file
/// ::opgs16::
#include <Shader\shader.h>

namespace opgs16 {
namespace element {

void CShaderWrapper::SetShader(CShaderNew * const shader) {
    m_shader = shader;
    m_is_useable = true;
}

void CShaderWrapper::UseShader() {
    if (m_is_useable) {
        m_shader->Use();
        RefreshUniformValues();
    }
}

void CShaderWrapper::RefreshUniformValues() {
    // Float
    for (const auto& [name, value] : m_parameters.m_floats)
        m_shader->SetFloat(name, value);
    // Matrix 4x4
    for (const auto& [name, value] : m_parameters.m_mat4s)
        m_shader->SetVecMatrix4f(name, value);
    // Vector2
    for (const auto&[name, value] : m_parameters.m_vec2)
        m_shader->SetVec2f(name, value);
    // Vector3
    for (const auto&[name, value] : m_parameters.m_vec3)
        m_shader->SetVec3f(name, value);
    // Int
    // Int
    for (const auto&[name, value] : m_parameters.m_ints)
        m_shader->SetInt(name, value);
    // IntPtr
    for (const auto&[name, value] : m_parameters.m_int_ptr)
        m_shader->SetIntPtr(name, value.first, value.second);
}

} /*! opgs16::element */
} /*! opgs16 */

