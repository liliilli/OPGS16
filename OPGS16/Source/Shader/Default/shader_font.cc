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

/*!
 * @file System/Shader/Default/Private/shader_font.cc
 * @brief Implementation file of ../Public/shader_font.h
 * @author Jongmin Yun
 * @log
 * 2018-04-03 Create file.
 */

#include <Shader\Default\shader_font.h> /// Header file
/// ::opgs16::resource::EShaderType
#include <Manager\resource_type.h>
/// import logger
#include <Headers\import_logger.h>

namespace {
constexpr const char* shader_vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 opPosition;\n"
"layout (location = 1) in vec3 opNormal;\n"
"layout (location = 2) in vec3 opTangent;\n"
"layout (location = 3) in vec2 opTexCoord;\n"
"out VS_OUT {"
"	vec2 texCoord;"
"} vs_out;"
"uniform mat4 projection;"
"void main() {\
	gl_Position = projection * vec4(opPosition.xy, 0, 1);\
	vs_out.texCoord = opTexCoord.xy;\
}";

constexpr const char* shader_fragment_shader =
"#version 430 core\n\
out vec4 color;\
in VS_OUT {\
	vec2 texCoord;\
} fs_in;\
\
layout (binding = 0) uniform sampler2D text;\
uniform vec3 textColor;\
\
void main() {\
	color = vec4(textColor, 1) * vec4(1, 1, 1, texture(text, fs_in.texCoord).r);\
}";

} /*! unnamed namespace */

namespace opgs16 {
namespace builtin {
namespace shader {

const char* SGlobalFont2D::s_shader_name = "gCommonFont";

SGlobalFont2D::SGlobalFont2D() {
    using resource::EShaderType;
    std::pair<EShaderType, const char* const> shader_list[2] {
        {EShaderType::VS, shader_vertex_shader}, {EShaderType::FS, shader_fragment_shader}
    };

    for (auto& [type, source] : shader_list) {
        m_shaders[type] = glCreateShader((GLenum)type);
        glShaderSource(m_shaders[type], 1, &source, nullptr);
        glCompileShader(m_shaders[type]);

#if defined(_DEBUG) /*! Error check and Log */
        int success{ 0 };
        glGetShaderiv(m_shaders[type], GL_COMPILE_STATUS, &success);
        if (!success) {
            PUSH_LOG_ERRO("Shader did not be compiled properly.");
            char info_log[0x200]{};
            glGetShaderInfoLog(m_shaders[type], 0x200, nullptr, info_log);
            PUSH_LOG_ERRO(info_log);
        }
        else {
            char log[100] = { "SGlobalQuad2D gCommonFont " };
            switch (type) {
            case EShaderType::VS:
              strcat(log, "vertex shader");
              break;
            case EShaderType::FS:
              strcat(log, "fragment shader");
              break;
            default: break;
            }
            strcat(log, " compile complete.");
            PUSH_LOG_ERRO(log);
        }
#endif
    }

  LinkShaderProgram();
}

} /*! opgs16::builtin::shader */
} /*! opgs16::builtin */
} /*! opgs16 */