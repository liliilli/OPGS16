#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/shader.cc
/// @brief Shader class type.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-08 Add comment.
/// 2018-08-11 Refactoring.
///

/// Header file
#include <Shader/shader.h>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

#include <Headers/import_logger.h>
#include <Manager/resource_type.h>
#include <Manager/Internal/shader_builtin_keywords.h>
#include <Phitos/Dbg/assert.h>

namespace {

void PrintShaderErrorLog(GLuint shader_id) {
  GLint max_length = 0;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

  std::vector<GLchar> info_log(max_length);
  glGetShaderInfoLog(shader_id, max_length, &max_length, &info_log[0]);
  PUSH_LOG_CRITICAL_EXT("Failed to compile shader. Log : {}", &info_log[0]);
}

void PrintShaderProgramErrorLog(GLuint program_id) {
  GLint max_length = 0;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);

  std::vector<GLchar> info_log(max_length);
  glGetProgramInfoLog(program_id, max_length, &max_length, &info_log[0]);
  PUSH_LOG_CRITICAL_EXT("Failed to compile shader. Log : {}", &info_log[0]);
}

} /// unnamed namespace

namespace opgs16::element {

CShaderNew::~CShaderNew() {
  if (!m_shaders.empty())
    m_shaders.clear();
  glDeleteProgram(m_program_id);
}

void CShaderNew::Use() {
  if (m_linked) {
    glUseProgram(m_program_id);
    return;
  }

  PUSH_LOG_ERRO("Failed load shader, you must to link the shader before using it.");
  glUseProgram(0);
}

CShaderNew* CShaderNew::SetShader(const opgs16::resource::SShader& shader_information) {
  for (auto& [type, shader_path] : shader_information.GetShaderList()) {
		pSetShader(type, shader_path);
	}

  pLinkShaderProgram();
  InitializeUniformVariables(shader_information.GetVariableList());
  return this;
}

void CShaderNew::pSetShader(resource::EShaderType shader_type, const std::string& path) {
  if (m_linked || pIsShaderCompiled(shader_type)) {
    PUSH_LOG_ERROR_EXT("Shader path already compiled.");
  }

  m_shaders[shader_type] = glCreateShader(static_cast<GLenum>(shader_type));
  std::string code  = ReadShaderCodeFrom(path);
  auto code_ptr     = code.c_str();
  const auto shader_source = m_shaders[shader_type];
  glShaderSource(shader_source, 1, &code_ptr, nullptr);
  glCompileShader(shader_source);

  // Log buffer setting to check compilation result of each shader fragment.
  GLint success = 0;
  glGetShaderiv(shader_source, GL_COMPILE_STATUS, &success);
  if (!success) {
    PrintShaderErrorLog(shader_source);
  }
}

void CShaderNew::pLinkShaderProgram() {
  m_program_id = glCreateProgram();

  for (auto& compiled_shader : m_shaders) {
    glAttachShader(m_program_id, compiled_shader.second);
  }
  glLinkProgram(m_program_id);

  for (auto& compiled_shader : m_shaders) {
    glDetachShader(m_program_id, compiled_shader.second);
    //glDeleteShader(compiled_shader.second);
  }
  m_shaders.clear();

  GLint is_linked = 0;
  glGetProgramiv(m_program_id, GL_LINK_STATUS, &is_linked);
  if (!is_linked) {
    PrintShaderProgramErrorLog(m_program_id);
  }

  m_linked = true;
}

void CShaderNew::InitializeUniformVariables(const resource::SShader::TVariableList& uniforms) {
  using opgs16::builtin::s_shader_builtin;
  using opgs16::builtin::s_shader_builtin_type;

  int32_t count = 0;
  glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &count);
  GLsizei length = 0;
  GLint size = 0;
  GLenum type = 0;

  for (int32_t i = 0; i < count; i++) {
    GLchar name[0x20] = { '\0', };
    glGetActiveUniform(m_program_id, (GLuint)i, 0x20, &length, &size, &type, name);
    auto id = glGetUniformLocation(m_program_id, name);
    PUSH_LOG_INFO_EXT("Uniform #{} Type: {} Name: {} Id : {}", i, type, name, id);
  }

  int32_t i = 0;
  for (const char* variable : s_shader_builtin) {
    const auto uniform_id = glGetUniformLocation(m_program_id, variable);
    if (uniform_id >= 0) {
      m_uniform_variables.emplace_back(std::make_tuple(
          variable, s_shader_builtin_type[i], uniform_id)
      );
    }
    ++i;
  }

  for (const auto& [variable_name, type] : uniforms) {
    const auto uniform_id = glGetUniformLocation(m_program_id, variable_name.c_str());
    if (uniform_id >= 0) {
      m_uniform_variables.emplace_back(std::make_tuple(
          variable_name, type, uniform_id)
      );
    }
  }
}

std::string CShaderNew::ReadShaderCodeFrom(const std::string& path) {
  std::string code;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(path);
    std::stringstream stream;

    // Read file and bring to each stream
    stream << file.rdbuf();
    file.close();

    // Convert stream into plain text string
    code = stream.str();
  }
  catch (std::ifstream::failure err) {
    PUSH_LOG_ERRO("Failed to read shader-path or string.");
    PHITOS_UNEXPECTED_BRANCH();
  }

  return code;
}

const std::vector<CShaderNew::TUniformStruct>&
CShaderNew::GetUniformVariableContainer() const noexcept {
  return m_uniform_variables;
}

void CShaderNew::SetInt(int32_t uniform_id, int value) const {
  glUniform1i(uniform_id, value);
}

void CShaderNew::SetIntPtr(int32_t uniform_id, int* value, int amount) const {
  glUniform1iv(uniform_id, amount, (GLint*)value);
}

void CShaderNew::SetFloat(int32_t uniform_id, float value) const {
  glUniform1f(uniform_id, value);
}

void CShaderNew::SetVec2f(int32_t uniform_id, const glm::vec2& vector) {
  glUniform2fv(uniform_id, 1, glm::value_ptr(vector));
}

void CShaderNew::SetVec3f(int32_t uniform_id, const glm::vec3& vector) {
  glUniform3fv(uniform_id, 1, glm::value_ptr(vector));
}

void CShaderNew::SetVecMatrix4f(int32_t uniform_id, const glm::mat4& matrix) {
  glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix));
}

void CShaderNew::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), value);
}

void CShaderNew::SetIntPtr(const std::string& name, int* value, int amount) const {
  const auto location = glGetUniformLocation(m_program_id, name.c_str());
  glUniform1iv(location, amount, (GLint*)value);
}

void CShaderNew::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(m_program_id, name.c_str()), value);
}

void CShaderNew::SetVec2f(const std::string& name, const glm::vec2& vector) {
  glUniform2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(vector));
}

void CShaderNew::SetVec3f(const std::string& name,
                          const float _1, const float _2, const float _3) {
  glUniform3f(glGetUniformLocation(m_program_id, name.c_str()), _1, _2, _3);
}

void CShaderNew::SetVec3f(const std::string& name, const glm::vec3& vector) {
  glUniform3fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(vector));
}

void CShaderNew::SetVecMatrix4f(const std::string& name, const glm::mat4& matrix) {
  const auto uniform = glGetUniformLocation(m_program_id, name.c_str());
  glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

//
//void CShaderNew::SetStructDirLight(const std::string & name, const light::DirectionalLight & container) {
//    SetVec3f(name + ".direction", container.GetDirection());
//
//    SetVec3f(name + ".ambient", container.GetAmbient());
//    SetVec3f(name + ".diffuse", container.GetDiffuse());
//    SetVec3f(name + ".specular", container.GetSpecular());
//}
//
//void CShaderNew::SetStructPointLight(const std::string & name, const light::PointLight & container) {
//    SetVec3f(name + ".position", container.GetLocalPosition());
//
//    SetVec3f(name + ".ambient", container.GetAmbient());
//    SetVec3f(name + ".diffuse", container.GetDiffuse());
//    SetVec3f(name + ".specular", container.GetSpecular());
//
//    SetFloat(name + ".constant", container.GetConstant());
//    SetFloat(name + ".linear", container.GetLinear());
//    SetFloat(name + ".quadratic", container.GetQuadratic());
//}
//
//void CShaderNew::SetStructSpotlight(const std::string & name, const light::Spotlight & container) {
//    SetVec3f(name + ".direction", container.direction);
//    SetVec3f(name + ".position", container.position);
//
//    SetVec3f(name + ".ambient", container.ambient);
//    SetVec3f(name + ".diffuse", container.diffuse);
//    SetVec3f(name + ".specular", container.specular);
//
//    SetFloat(name + ".inCutoff", container.inCutOff);
//    SetFloat(name + ".outCutoff", container.outCutOff);
//
//    SetFloat(name + ".constant", container.constant);
//    SetFloat(name + ".linear", container.linear);
//    SetFloat(name + ".quadratic", container.quadratic);
//}
//
} /*! opgs16::element */
