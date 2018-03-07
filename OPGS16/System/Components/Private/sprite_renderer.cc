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
 */

#include "../Public/sprite_renderer.h"          /*! Header file */
#include "../Impl/sprite_renderer_impl.h"       /*! SpriteRendererImpl */

namespace opgs16 {
namespace component {

Sprite2DRenderer::Sprite2DRenderer(const std::string& sprite_tag,
                               const std::string& shader_tag,
                               const opgs16::resource::Texture2D::IndexSize& texture_index,
                               const unsigned layer) :
    m_impl { new SpriteRendererImpl(sprite_tag, shader_tag, texture_index, layer) } {
}

void Sprite2DRenderer::SetLayer(const unsigned layer) {
    m_impl->SetLayer(layer);
}

unsigned Sprite2DRenderer::GetLayer() const {
    return m_impl->GetLayer();
}

ShaderWrapper& Sprite2DRenderer::GetWrapper() const {
    return m_impl->GetWrapper();
}

const Sprite2DRenderer::IndexSize& Sprite2DRenderer::GetTextureIndex() const noexcept {
    return m_impl->GetTextureIndex();
}

void Sprite2DRenderer::SetTextureIndex(const IndexSize& new_index) {
    m_impl->SetTextureIndex(new_index);
}

void Sprite2DRenderer::RenderSprite() {
    m_impl->RenderSprite();
}

Sprite2DRenderer::~Sprite2DRenderer() = default;

} /*! opgs16::component */
} /*! opgs16 */

