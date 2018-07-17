
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
uniform mat4 opProj;\
uniform mat4 opModel;\
\
void main() {\
	gl_Position		  = opProj * opModel * vec4(opPosition.xy, 0, 1);\
	vs_out.texCoord = opTexCoord;\
}";

constexpr const char* shader_fragment_shader =
"#version 430 core\n\
\n\
out vec4 color;\
\
layout (binding = 0) uniform sampler2D uTexture1;\
uniform vec2 uTexelLD;\
uniform vec2 uTexelRU;\
uniform vec2 opScale = vec2(1, 1);\
uniform vec2 opOffset = vec2(0, 0);\
uniform float opAlpha;\
\
in VS_OUT {\
	vec2 texCoord;\
} fs_in;\
\
vec2 GetTransformedTexel() {\
  vec2 uTexelRegion = uTexelRU - uTexelLD;\
  vec2 finalTexel = mod((fs_in.texCoord / opScale), 1.0);\
  finalTexel += opOffset;\
  return uTexelLD + mod(finalTexel, 1.0) * uTexelRegion;\
};\
\
void main() {\
	color = texture(uTexture1, GetTransformedTexel()) * vec4(1, 1, 1, opAlpha);\
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

  LinkShaderProgram();
}

} /// ::opgs16::builtin::shader namespace