#include <precompiled.h>
/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Private/shader.h
 * @brief Shader program.
 * @author Jongmin Yun
 * @log
 * 2018-04-08 Add comment.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/// Header file
#include <Shader/shader.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <glm/gtc/type_ptr.hpp>

#include <Headers/import_logger.h>
#include <Manager/resource_type.h>
#include <Manager/Internal/shader_builtin_keywords.h>
#include <Phitos/Dbg/assert.h>

constexpr unsigned LOG_SIZE = 0x200;

namespace opgs16::element {

void CShaderNew::Use() {
  if (m_linked) {
    glUseProgram(m_program_id);
    return;
  }

  PUSH_LOG_ERRO("Failed load shader, you must to link the shader before using it.");
  glUseProgram(0);
}

CShaderNew*
CShaderNew::SetShader(const opgs16::resource::SShader& shader_information) {
  for (auto& [type, shader_path] : shader_information.GetShaderList()) {
		SetShader(type, shader_path);
	}

  LinkShaderProgram();

  InitializeUniformVariables(shader_information.GetVariableList());
  return this;
}

void CShaderNew::SetShader(resource::EShaderType shader_type,
                           const std::string& path) {
  if (m_linked || IsCompiled(shader_type)) {
    PUSH_LOG_ERROR_EXT("Shader path already compiled.");
  }

  m_shaders[shader_type] = glCreateShader((GLenum)shader_type);
  std::string code = ReadShaderCodeFrom(path);
  auto code_ptr = code.c_str();
  auto source   = m_shaders[shader_type];

  glShaderSource(source, 1, &code_ptr, nullptr);
  glCompileShader(source);

  /// Log buffer setting to check compilation result of each shader fragment.
  int success = 0;
  glGetShaderiv(source, GL_COMPILE_STATUS, &success);
  char info_log[LOG_SIZE]{};
  if (!success) {
    ShaderErrorPrint(m_shaders[shader_type], info_log);
  }
}

CShaderNew::~CShaderNew() {
  if (!m_shaders.empty())
    m_shaders.clear();

  glDeleteProgram(m_program_id);
}

void CShaderNew::LinkShaderProgram() {
  m_program_id = glCreateProgram();

  for (auto& compiled_shader : m_shaders) {
    glAttachShader(m_program_id, compiled_shader.second);
  }

  glLinkProgram(m_program_id);

  for (auto& compiled_shader : m_shaders) {
    glDetachShader(m_program_id, compiled_shader.second);
    glDeleteShader(compiled_shader.second);
  }
  m_shaders.clear();

  int success = 0;
  glGetShaderiv(m_program_id, GL_COMPILE_STATUS, &success);

  char info_log[LOG_SIZE];
  if (!success) {
    ShaderErrorPrint(m_program_id, info_log);
  }

  m_linked = true;
}

void CShaderNew::InitializeUniformVariables(
    const resource::SShader::TVariableList& uniforms) {
  using opgs16::builtin::s_shader_builtin;
  using opgs16::builtin::s_shader_builtin_type;

  int32_t count = 0;
  glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &count);
  GLsizei length = 0;
  GLint size = 0;
  GLenum type = 0;

  for (int32_t i = 0; i < count; i++) {
    GLchar name[0x20];
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

void CShaderNew::ShaderErrorPrint(GLuint shader, char* info_log) {
  glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
  PUSH_LOG_ERROR_EXT("Failed to compile shader. {}", info_log);
}

void CShaderNew::LinkingErrorPrint(GLuint shader, char* info_log) {
  glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
  PUSH_LOG_ERROR_EXT("Failed to link shader program. {}", info_log);
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

//
//namespace helper {
//
///**
// * @brief Old Shader wrapper class
// *
// * @deprecated
// */
//Shader::Shader(const GLchar * vertex_path, bool link) {
//    std::string vertex_code;
//    std::ifstream vertex_shader_file;
//    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//    try {
//        vertex_shader_file.open(vertex_path);
//        std::stringstream vertex_shader_stream;
//
//        // Read file and bring to each stream
//        vertex_shader_stream << vertex_shader_file.rdbuf();
//        vertex_shader_file.close();
//
//        // Convert stream into plain text string
//        vertex_code = vertex_shader_stream.str();
//    }
//    catch (std::ifstream::failure err) {
//        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
//    }
//
//    // Setting
//    auto vertex_shader_code = vertex_code.c_str();
//
//    // Log buffer setting to check compilation result of each shader program.
//    int success = 0;
//    char info_log[LOG_SIZE];
//
//    // Vertex Shader compile
//    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
//    glCompileShader(vertex_shader);
//
//    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
//    if (!success) ShaderErrorPrint(vertex_shader, info_log);
//
//    if (link) {
//        // Shader program object creation and linking
//        kId = glCreateProgram();
//        {
//            glAttachShader(kId, vertex_shader);
//            glDeleteShader(vertex_shader);
//        }
//    }
//
//}
//
//Shader::Shader(const GLchar * vertex_path, const GLchar * fragment_path, bool link) {
//    // Retrieve the vertex / fragment source code from each file path
//    std::string vertex_code;
//    std::string fragment_code;
//
//    std::ifstream vertex_shader_file;
//    std::ifstream fragment_shader_file;
//
//    // Ensure ifstream m_object_list can throw exceptions
//    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//    try {
//        // Open Files
//        vertex_shader_file.open(vertex_path);
//        fragment_shader_file.open(fragment_path);
//
//        std::stringstream vertex_shader_stream;
//        std::stringstream fragment_shader_stream;
//
//        // Read file and bring to each stream
//        vertex_shader_stream << vertex_shader_file.rdbuf();
//        fragment_shader_stream << fragment_shader_file.rdbuf();
//
//        // Close file handlers
//        vertex_shader_file.close();
//        fragment_shader_file.close();
//
//        // Convert stream into plain text string
//        vertex_code = vertex_shader_stream.str();
//        fragment_code = fragment_shader_stream.str();
//    }
//    catch (std::ifstream::failure err) {
//        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
//    }
//
//    // Setting
//    auto vertex_shader_code = vertex_code.c_str();
//    auto fragment_shader_code = fragment_code.c_str();
//
//    // Log buffer setting to check compilation result of each shader program.
//    int success = 0;
//    char info_log[LOG_SIZE];
//
//    // Vertex Shader compile
//    {
//        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
//        glCompileShader(vertex_shader);
//
//        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
//        if (!success) ShaderErrorPrint(vertex_shader, info_log);
//    }
//
//    // Fragment Shader compile
//    {
//        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
//        glCompileShader(fragment_shader);
//
//        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
//        if (!success) ShaderErrorPrint(fragment_shader, info_log);
//    }
//
//    if (link) {
//        // Shader program object creation and linking
//        kId = glCreateProgram();
//        {
//            glAttachShader(kId, vertex_shader);
//            glAttachShader(kId, fragment_shader);
//            glLinkProgram(kId);
//
//            glDeleteShader(vertex_shader);
//            glDeleteShader(fragment_shader);
//
//            glGetShaderiv(kId, GL_COMPILE_STATUS, &success);
//            if (!success) ShaderErrorPrint(kId, info_log);
//        }
//    }
//}
//
//Shader::Shader(const GLchar* vertex_path,
//               const GLchar* geometry_path,
//               const GLchar* fragment_path) : Shader{ vertex_path, fragment_path, false } {
//    std::string geometry_code;
//
//    std::ifstream geometry_shader_file;
//    geometry_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//    try {
//        geometry_shader_file.open(geometry_path);
//
//        std::stringstream geometry_stream;
//        geometry_stream << geometry_shader_file.rdbuf();
//
//        geometry_shader_file.close();
//        geometry_code = geometry_stream.str();
//    }
//    catch (std::ifstream::failure err) {
//        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ" << std::endl;
//    }
//
//    // Log buffer setting to check compilation result of each shader program.
//    int success = 0;
//    char info_log[LOG_SIZE];
//
//    // Shader compile
//    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
//    {
//        auto code = geometry_code.c_str();
//        glShaderSource(geometry_shader, 1, &code, nullptr);
//        glCompileShader(geometry_shader);
//
//        glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
//        if (!success) ShaderErrorPrint(geometry_shader, info_log);
//    }
//
//    // Linking
//    // Shader program object creation and linking
//    kId = glCreateProgram();
//    {
//        glAttachShader(kId, vertex_shader);
//        glAttachShader(kId, geometry_shader);
//        glAttachShader(kId, fragment_shader);
//        glLinkProgram(kId);
//
//        glDeleteShader(vertex_shader);
//        glDeleteShader(geometry_shader);
//        glDeleteShader(fragment_shader);
//
//        glGetShaderiv(kId, GL_COMPILE_STATUS, &success);
//        if (!success) ShaderErrorPrint(kId, info_log);
//    }
//}
//
//void Shader::Use() {
//    // Just use kId to load associated with shader program object.
//    glUseProgram(kId);
//}
//
//void Shader::SetBool(const std::string & name, bool value) const {
//    glUniform1i(glGetUniformLocation(kId, name.c_str()), static_cast<GLint>(value));
//}
//
//void Shader::SetInt(const std::string & name, int value) const {
//    glUniform1i(glGetUniformLocation(kId, name.c_str()), value);
//}
//
//void Shader::SetFloat(const std::string & name, float value) const {
//    glUniform1f(glGetUniformLocation(kId, name.c_str()), value);
//}
//
//void Shader::SetVec3f(const std::string & name,
//                      const float _1, const float _2, const float _3) {
//    glUniform3f(glGetUniformLocation(kId, name.c_str()), _1, _2, _3);
//}
//
//void Shader::SetVec3f(const std::string& name, const glm::vec3& vector) {
//    glUniform3fv(glGetUniformLocation(kId, name.c_str()), 1, glm::value_ptr(vector));
//}
//
//void Shader::SetVecMatirix4f(const std::string& name,
//                             glm::mat4& matrix) {
//    glUniformMatrix4fv(glGetUniformLocation(kId, name.c_str()),
//                       1, GL_FALSE, glm::value_ptr(matrix));
//}
//
//void Shader::SetStructDirLight(const std::string& name, const light::DirectionalLight& container) {
//    SetVec3f(name + ".direction", container.GetDirection());
//
//    SetVec3f(name + ".ambient", container.GetAmbient());
//    SetVec3f(name + ".diffuse", container.GetDiffuse());
//    SetVec3f(name + ".specular", container.GetSpecular());
//}
//
//void Shader::SetStructPointLight(const std::string& name, const light::PointLight& container) {
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
//void Shader::SetStructSpotlight(const std::string& name, const light::Spotlight& container) {
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
//void Shader::ShaderErrorPrint(GLuint shader, char* info_log) {
//    glGetShaderInfoLog(shader, LOG_SIZE, nullptr, info_log);
//    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
//}
//
//}
