#ifndef OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H
#define OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H

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
 * @file System/Element/Internal/texture_internal.h
 * @brief ::opgs16::element::texture internal type implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-11 Create file
 */

#include <GL\glew.h>

namespace opgs16 {
namespace element {
namespace texture {
namespace _internal {

/*! Inner struct used in class Texture2D methods, SetTextureParameterI. */
struct TextureParameter {
    GLint option;   /*! Option of m_texture parameter, such as GL_TEXTURE_MIN_FILTER, etc. */
    GLint mode;     /*! Mode of m_texture parameter option, like a GL_NEAREST, GL_REPEAT. */
};

} /*! opgs16::element::texture::_internal */
} /*! opgs16::element::texture */
} /*! opgs16::element */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_ELEMENT_INTERNAL_TEXTURE_INTERNAL_H


