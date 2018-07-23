#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/Default/aabb_2d_line.cc
///
/// @brief
/// AABB 2d box rendering shader program class file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-07 Create file.
///

/// Header file
#include <Shader/Default/aabb_2d_line.h>
/// ::opgs16::resource::EShaderType
#include <Manager/resource_type.h>
/// import logger
#include <Headers/import_logger.h>

namespace {

constexpr const char* vertex_shader_source =
"#version 430 core\n"
"layout (location = 0) in vec3 vertex;\n"
"uniform mat4 u_view;\n"
"uniform mat4 u_projection;\n"
"void main() {\n"
"  gl_Position = u_projection * u_view * vec4(vertex, 1.f);\n"
"}";

constexpr const char* fragment_shader_source =
"#version 430 core\n"
"uniform vec3 u_color;\n"
"void main() {\n"
"  gl_FragColor = vec4(u_color, 1.f);\n"
"}";

} /// unnamed namespace

namespace opgs16::builtin::shader {

SAABB2DShader::SAABB2DShader() {
  using resource::EShaderType;
  std::pair<EShaderType, const char*> shader_list[2] {
      {EShaderType::VS, vertex_shader_source},
      {EShaderType::FS, fragment_shader_source}
  };

  for (auto& [type, source] : shader_list) {
    m_shaders[type] = glCreateShader(static_cast<GLenum>(type));
    glShaderSource(m_shaders[type], 1, &source, nullptr);
    glCompileShader(m_shaders[type]);
  }

  LinkShaderProgram();
}

} /// ::opgs16::builtin::shader namespace
