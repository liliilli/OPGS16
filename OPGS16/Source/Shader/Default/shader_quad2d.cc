
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Shader/Default/shader_quad2d.cc
///
/// @brief Implementation file of SGlobalQuad2D
///
/// @author Jongmin Yun
///
/// @log
/// 2018-04-03 Create shader.
/// 2018-06-14 Update shader to have vao binding.
///

/// Header file
#include <Shader/Default/shader_quad2d.h>
/// ::opgs16::resource::EShaderType
#include <Manager/resource_type.h>
/// import logger
#include <Headers/import_logger.h>

namespace {

constexpr const char* shader_vertex_shader =
"#version 430 core\n"
"\n"
"layout (location = 0) in vec3 opPosition;\n"
"layout (location = 1) in vec3 opNormal;\n"
"layout (location = 2) in vec3 opTangent;\n"
"layout (location = 3) in vec2 opTexCoord;\n"
"\n\
out VS_OUT {\
	vec2 texCoord;\
} vs_out;\
\
uniform mat4 projection;\
\
void main() {\
	gl_Position		  = projection * vec4(opPosition.xy, 0, 1);\
	vs_out.texCoord = opTexCoord;\
}";

constexpr const char* shader_fragment_shader =
"#version 430 core\n\
\n\
out vec4 color;\
uniform vec2 uTexelLD;\
uniform vec2 uTexelRU;\
\
in VS_OUT {\
	vec2 texCoord;\
} fs_in;\
\
layout (binding = 0) uniform sampler2D uTexture1;\
uniform float alpha;\
\
void main() {\
	color = texture(uTexture1, uTexelLD + (fs_in.texCoord * (uTexelRU - uTexelLD))) * vec4(1, 1, 1, alpha);\
}";

} /*! unnamed namespace */

namespace opgs16::builtin::shader {

const char* SGlobalQuad2D::s_shader_name = "gQuad";

SGlobalQuad2D::SGlobalQuad2D() {
  using resource::EShaderType;
  std::pair<EShaderType, const char* const> shader_list[2]{
      {EShaderType::VS, shader_vertex_shader},
      {EShaderType::FS, shader_fragment_shader}
  };

  for (auto&[type, source] : shader_list) {
    m_shaders[type] = glCreateShader(static_cast<GLenum>(type));
    glShaderSource(m_shaders[type], 1, &source, nullptr);
    glCompileShader(m_shaders[type]);

#if defined(false)
#if defined(_DEBUG) /*! Error check and Log */
    int success{ 0 };
    glGetShaderiv(m_shaders[type], GL_COMPILE_STATUS, &success);
    if (!success) {
      PushLog(LOG_TYPE_ERRO, L"Shader did not be compiled properly.");
      char info_log[log_size]{};
      glGetShaderInfoLog(m_shaders[type], log_size, nullptr, info_log);
      PushLog(LOG_TYPE_ERRO, info_log);
    }
    else {
      wchar_t log[100] = { L"SGlobalQuad2D gQuad " };
      switch (type) {
      case EShaderType::VS:
        wcscat(log, L"vertex shader");   break;
      case EShaderType::FS: wcscat(log, L"fragment shader");  break;
      default: break;
      }
      wcscat(log, L" compile complete.");
      PushLog(LOG_TYPE_INFO, log);
    }
#endif
#endif
  }
}

} /// ::opgs16::builtin::shader namespace