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

/*!
 * @file System/Shader/Default/Private/shader_quad2d.cc
 * @brief Implementation file of ../Public/shader_quad2d.h
 * @author Jongmin Yun
 * @log
 * 2018-04-03 Create shader.
 */

#include <Shader\Default\shader_quad2d.h>   /// Header file
/// ::opgs16::resource::EShaderType
#include <Manager\resource_type.h>
/// import logger
#include <Headers\import_logger.h>

namespace {
constexpr unsigned log_size = 0x200;

constexpr const char* shader_vertex_shader =
"#version 430 core\n\
\n\
out VS_OUT {\
	vec2 texCoord;\
} vs_out;\
\
uniform mat4 projection;\
\
void main() {\
	const vec2 vertices[] = vec2[](\
		vec2(-1, -1), vec2(1, -1), vec2(-1, 1),\
		vec2(-1, 1), vec2(1, -1), vec2(1, 1)\
	);\
    \
	gl_Position		= projection * vec4(vertices[gl_VertexID], 0, 1);\
	vs_out.texCoord = (vertices[gl_VertexID] + vec2(1)) / 2;\
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

namespace opgs16 {
namespace builtin {
namespace shader {

const char* SGlobalQuad2D::s_shader_name = "gQuad";

SGlobalQuad2D::SGlobalQuad2D() {
    using resource::EShaderType;
    std::pair<EShaderType, const char* const> shader_list[2] {
        {EShaderType::VS, shader_vertex_shader}, {EShaderType::FS, shader_fragment_shader}
    };

    for (auto& [type, source] : shader_list) {
        m_shaders[type] = glCreateShader((GLenum)type);
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

} /*! opgs16::builtin::shader */
} /*! opgs16::builtin */
} /*! opgs16 */