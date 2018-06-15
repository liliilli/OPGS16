
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/shader_wrapper.cc
///
/// @brief Definition file of ./shader_wrapper.h
///
/// @log
/// 2018-04-17 Move definition files to ::opgs16::element.
/// 2018-06-14 Update
///

/// Header file
#include <Shader/shader_wrapper.h>

/// ::opgs16::element::DMeshVector
#include <Element/Internal/mesh_vector.h>
/// ::opgs16::element::CVaoContainer
#include <Element/Internal/vao_container.h>
/// ::opgs16::element::CShaderNew
#include <Shader/shader.h>

/// ::phitos Expanded assertion.
#include <Phitos/Dbg/assert.h>

namespace opgs16::element {

void CShaderWrapper::SetShader(CShaderNew* const shader) {
  m_shader = shader;
  m_is_useable = true;
}

void CShaderWrapper::UseShader() {
  if (m_is_useable) {
    m_shader->Use();
    RefreshUniformValues();
  }
}

void CShaderWrapper::SetAttribute(const CVaoContainer* vao_container) {
  PHITOS_ASSERT(m_is_useable == true, "Shader is not set up.");

  m_shader->Use();
  constexpr uint32_t stride = sizeof DMeshVector;

  for (const auto& vao : vao_container->GetVaoList()) {
    glBindVertexArray(vao.GetVaoId());
    glBindBuffer(GL_ARRAY_BUFFER, vao.GetVboId());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3 * 2));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof DVector3 * 3));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
  }

  glUseProgram(0);
}

void CShaderWrapper::RefreshUniformValues() {
  // Float
  for (const auto&[name, value] : m_parameters.m_floats)
    m_shader->SetFloat(name, value);
  // Matrix 4x4
  for (const auto&[name, value] : m_parameters.m_mat4s)
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

} /// ::opgs16::element namespace

