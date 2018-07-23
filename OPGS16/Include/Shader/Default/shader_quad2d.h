#include <precompiled.h>
#ifndef OPGS16_SHADER_DEFAULT_SHADER_QUAD2D_H
#define OPGS16_SHADER_DEFAULT_SHADER_QUAD2D_H

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
 * @file System/Shader/Default/Public/shader_quad2d.h
 * @brief OPGS16 Built-in shader represents plain 2d quad image.
 * @author Jongmin Yun
 * @log
 * 2018-04-03 Create shader.
 */

 /// ::opgs16::element::CShaderNew
#include <Shader\shader.h>

namespace opgs16 {
namespace builtin {
namespace shader {

class SGlobalQuad2D final : public element::CShaderNew {
public:
    SGlobalQuad2D();
    static const char* s_shader_name;
};


} /*! opgs16::builtin::shader */
} /*! opgs16::builtin */
} /*! opgs16 */

#endif // OPGS16_SHADER_DEFAULT_SHADER_QUAD2D_H