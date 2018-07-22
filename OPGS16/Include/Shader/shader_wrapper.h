#ifndef OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H
#define OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/shader_wrapper.h
///
/// @brief Shader wrapper.
/// ���̴� �Ŵ������� ������ ���̴��� ���� Ȥ�� �����͸� ������, �� ���̴��� ���ŵ�
/// ������ �������� ���� ��ü�� ������.
///
/// �� ���� ��ü�� _������ Ȥ�� ����Ʈ ���μ��� �����ӿ� �������� �ٿ��� �� ������
/// (�ǵ��� ���ÿ� �ö� �� �ֵ��� ���� ��) Use() �Լ��� UpdateUniforms() �� �̿���
/// ���̴��� ���� ������ �������� ���ε��� �� �ֵ��� �Ѵ�.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-28 Add vector2 uniform value container and function related to vector2.
/// 2018-04-08 Supporting change of shader on running.
/// 2018-04-17 Move ::CShaderWrapper to ::opgs16::element::CShaderWrapper.
///

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <Manager/ResourceType/shader.h>
/// Forward declaration
#include <opgs16fwd.h>

namespace opgs16::element {
class CVaoContainer;
}

namespace opgs16::element {

class CShaderWrapper {
  using TUniformId = int32_t;
  using TUniformInformation = std::pair<resource::SShader::EVariableType,
                                        TUniformId>;
public:
  ///
  /// @brief Set shader.
  /// @param[in] shader
  ///
  void SetShader(CShaderNew* shader);

  ///
  /// @brief Use shader. If shader graps nothing, do nothing any more.
  ///
  void UseShader();

  ///
  /// @brief Set default attributes to shader.
  /// This function must be called after SetShader(shader).
  ///
  /// @param[in] vao_container Vao container to be bound.
  ///
  void SetAttribute(const CVaoContainer* vao_container);

  ///
  /// @brief Insert uniform variable value to be used by shader.
  ///
  /// @param[in] tag The tag to insert value.
  /// @param[in] value The value insert.
  ///
  /// @tparam TTy Type parameter to compare with supported uniform container's type.
  ///
  template <typename TTy>
  void SetUniformValue(const std::string& tag, const TTy& value);

  template <>
  void SetUniformValue<float>(const std::string& tag, const float& value) {
    using opgs16::resource::SShader;
    if (auto it = m_uniform_mapper.find(tag); it != m_uniform_mapper.end()) {
      if (it->second.first == SShader::EVariableType::Float) {
        m_parameters.m_floats[it->second.second] = value;
      }
    }
  }

  template <>
  void SetUniformValue<glm::mat4>(const std::string& tag, const glm::mat4& value) {
    using opgs16::resource::SShader;
    if (auto it = m_uniform_mapper.find(tag); it != m_uniform_mapper.end()) {
      if (it->second.first == SShader::EVariableType::Matrix4) {
        m_parameters.m_mat4s[it->second.second] = value;
      }
    }
  }

  template <>
  void SetUniformValue<glm::vec2>(const std::string& tag, const glm::vec2& value) {
    using opgs16::resource::SShader;
    if (auto it = m_uniform_mapper.find(tag); it != m_uniform_mapper.end()) {
      if (it->second.first == SShader::EVariableType::Vec2) {
        m_parameters.m_vec2[it->second.second] = value;
      }
    }
  }

  template <>
  void SetUniformValue<glm::vec3>(const std::string& var_name, const glm::vec3& value) {
    using opgs16::resource::SShader;
    if (auto it = m_uniform_mapper.find(var_name); it != m_uniform_mapper.end()) {
      if (it->second.first == SShader::EVariableType::Vec3) {
        m_parameters.m_vec3[it->second.second] = value;
      }
    }
  }

  template <>
  void SetUniformValue<int>(const std::string& var_name, const int& value) {
    using opgs16::resource::SShader;
    if (auto it = m_uniform_mapper.find(var_name); it != m_uniform_mapper.end()) {
      if (it->second.first == SShader::EVariableType::Int) {
        m_parameters.m_ints[it->second.second] = value;
      }
    }
  }

  void SetUniformFloat(const std::string& var_name, float value) noexcept;

  void SetUniformInt(const std::string& var_name, int32_t value) noexcept;

  void SetUniformMat4(const std::string& var_name, const glm::mat4& value) noexcept;

  void SetUniformVec2(const std::string& var_name, const glm::vec2& value) noexcept;

  void SetUniformVec3(const std::string& var_name, const glm::vec3& value) noexcept;

  void SetUniformIntPtr(const char* var_name, int32_t* pointer, int32_t amount) noexcept;

private:
  /// Set uniform variables of shader with new values.
  void RefreshUniformValues();

  /// Initialize uniform values whenever new shader set.
  void InitializeUnfiormValues();

  struct DParameters final {
    std::map<int32_t, float> m_floats;
    std::map<int32_t, int32_t> m_ints;
    std::map<int32_t, std::pair<int*, int>> m_int_ptr;
    std::map<int32_t, glm::vec2> m_vec2;
    std::map<int32_t, glm::vec3> m_vec3;
    std::map<int32_t, glm::mat4> m_mat4s;
  };

  /// Shader pointer retrieved from ShaderManager.
  CShaderNew* m_shader = nullptr;
  /// The flag for preventing using of nullptr shader.
  bool m_is_useable = false;
  /// Uniform parameters
  DParameters m_parameters;
  ///
  std::unordered_map<std::string, TUniformInformation> m_uniform_mapper;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_SYSTEM_SHADER_SHADER_WRAPPER_H