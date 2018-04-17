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
 * @file System/Components/Private/sprite_renderer.cc
 * @brief Definition file of sprite_renderer.h
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-07 Move file to /Component and Add boilerplate comments.
 * 2018-04-06 Abandon IndexSize structure indicates texture index, replace it with one unsigned value.
 */

#include "../Public/sprite_renderer.h"              /*! Header file */
#include "../Impl/sprite_renderer_impl.h"           /*! CSpriteRendererImpl */
#include "../../Element/Public/object.h"            /*! ::opgs16::element::CObject */

namespace opgs16 {
namespace component {

CSprite2DRenderer::CSprite2DRenderer(element::CObject& bind_object,
                                     const std::string& sprite_tag, const std::string& shader_tag,
                                     const unsigned texture_index, const unsigned render_layer) :
    CRendererBase{ bind_object, render_layer },
    m_impl { new _internal::CSpriteRendererImpl(sprite_tag, shader_tag, texture_index) } {
}

element::CShaderWrapper& CSprite2DRenderer::Wrapper() const {
    return m_impl->Wrapper();
}

void CSprite2DRenderer::SetTexture(const std::string& texture_name) {
    return m_impl->SetTexture(texture_name);
}

const unsigned CSprite2DRenderer::TextureIndex() const noexcept {
    return m_impl->TextureIndex();
}

// ReSharper disable CppMemberFunctionMayBeConst
void CSprite2DRenderer::SetTextureIndex(const unsigned index_value) {
    // ReSharper restore CppMemberFunctionMayBeConst
    m_impl->SetTextureIndex(index_value);
}

// ReSharper disable CppMemberFunctionMayBeConst
void CSprite2DRenderer::RenderSprite() {
    // ReSharper restore CppMemberFunctionMayBeConst
    m_impl->RenderSprite();
}

void CSprite2DRenderer::SetShader(const std::string& shader_name) {
    m_impl->SetShader(shader_name);
}

void CSprite2DRenderer::SetInstanceCount(unsigned instance_count) {
    m_impl->SetInstanceCount(instance_count);
}

CSprite2DRenderer::~CSprite2DRenderer() = default;

} /*! opgs16::component */
} /*! opgs16 */

