
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

#include <Manager/Internal/shader_builtin_keywords.h>
/// ::opgs16::element::CShaderNew
#include <Shader/shader.h>

/// ::phitos Expanded assertion.
#include <Phitos/Dbg/assert.h>

namespace opgs16::element {

void CShaderWrapper::SetShader(CShaderNew* shader) {
  if (shader) {
    m_shader     = shader;
    InitializeUnfiormValues();
    m_is_useable = true;
  }
  else {
    m_shader     = nullptr;
    m_is_useable = false;
  }
}

void CShaderWrapper::InitializeUnfiormValues() {
  decltype(m_uniform_mapper) m_previous_mapper = m_uniform_mapper;
  m_uniform_mapper.clear();
  const auto& uniforms = m_shader->GetUniformVariableContainer();

  for (auto& [var_name, type, uniform_id] : uniforms) {
    switch (type) {
    case resource::SShader::EVariableType::Matrix4:
      if (const auto it = m_previous_mapper.find(var_name);
          it != m_previous_mapper.end())
      {
        if (auto previous_id = it->second.second; uniform_id != previous_id) {
          m_parameters.m_mat4s[uniform_id] = m_parameters.m_mat4s[previous_id];
          m_parameters.m_mat4s.erase(previous_id);
        }
      }
      else {
        m_parameters.m_mat4s.emplace(uniform_id, glm::mat4{});
      }
      break;
    case resource::SShader::EVariableType::Vec3:
      if (const auto it = m_previous_mapper.find(var_name);
          it != m_previous_mapper.end())
      {
        if (auto previous_id = it->second.second; uniform_id != previous_id) {
          m_parameters.m_vec3[uniform_id] = m_parameters.m_vec3[previous_id];
          m_parameters.m_vec3.erase(previous_id);
        }
      }
      else {
        m_parameters.m_vec3.emplace(uniform_id, glm::vec3{});
      }
      break;
    case resource::SShader::EVariableType::Vec2: {
        glm::vec2 initial_value = glm::vec2{0};
        if (var_name == builtin::s_uniform_scale) {
          initial_value = glm::vec2{1};
        }

        if (const auto it = m_previous_mapper.find(var_name);
            it != m_previous_mapper.end())
        {
          if (auto previous_id = it->second.second; uniform_id != previous_id) {
            m_parameters.m_vec2[uniform_id] = m_parameters.m_vec2[previous_id];
            m_parameters.m_vec2.erase(previous_id);
          }
        }
        else {
          m_parameters.m_vec2.emplace(uniform_id, initial_value);
        }
      }
      break;
    case resource::SShader::EVariableType::Float: {
        float initial_value = 0.f;
        if (var_name == builtin::s_uniform_alpha) {
          initial_value = 1.0f;
        }
        if (const auto it = m_previous_mapper.find(var_name);
            it != m_previous_mapper.end())
        {
          if (auto previous_id = it->second.second; uniform_id != previous_id) {
            m_parameters.m_floats[uniform_id] = m_parameters.m_floats[previous_id];
            m_parameters.m_floats.erase(previous_id);
          }
        }
        else {
          m_parameters.m_floats.emplace(uniform_id, initial_value);
        }
      }

      break;
    case resource::SShader::EVariableType::Int:
      if (const auto it = m_previous_mapper.find(var_name);
          it != m_previous_mapper.end())
      {
        if (auto previous_id = it->second.second; uniform_id != previous_id) {
          m_parameters.m_ints[uniform_id] = m_parameters.m_ints[previous_id];
          m_parameters.m_ints.erase(previous_id);
        }
      }
      else {
        m_parameters.m_ints.emplace(uniform_id, 0);
      }
      break;
    case resource::SShader::EVariableType::IntPtr:
      if (const auto it = m_previous_mapper.find(var_name);
          it != m_previous_mapper.end())
      {
        if (auto previous_id = it->second.second; uniform_id != previous_id) {
          m_parameters.m_int_ptr[uniform_id] = m_parameters.m_int_ptr[previous_id];
          m_parameters.m_int_ptr.erase(previous_id);
        }
      }
      else {
        m_parameters.m_int_ptr.emplace(uniform_id, std::make_pair(nullptr, 0));
      }
      break;
    case resource::SShader::EVariableType::Vec4:
    case resource::SShader::EVariableType::Matrix3:
    case resource::SShader::EVariableType::IVec4:
    case resource::SShader::EVariableType::IVec3:
    case resource::SShader::EVariableType::IVec2:
    case resource::SShader::EVariableType::Texture2D:
      break;
    }

    m_uniform_mapper.emplace(var_name, std::make_pair(type, uniform_id));
  }
}

void CShaderWrapper::UseShader() {
  if (m_is_useable) {
    m_shader->Use();
    RefreshUniformValues();
  }
  else {
    PUSH_LOG_WARN("Shader is not bind so can not render because of initialization failure and etc.");
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                          (void*)(sizeof DVector3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
                          (void*)(sizeof DVector3 * 2));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride,
                          (void*)(sizeof DVector3 * 3));
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
  for (const auto&[name, value] : m_parameters.m_ints)
    m_shader->SetInt(name, value);
  // IntPtr
  for (const auto&[name, value] : m_parameters.m_int_ptr)
    m_shader->SetIntPtr(name, value.first, value.second);
}

} /// ::opgs16::element namespace

