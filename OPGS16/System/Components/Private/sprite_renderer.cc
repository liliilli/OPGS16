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

#include "../Public/sprite_renderer.h"              /*! Header file */
#include "../Impl/sprite_renderer_impl.h"           /*! SpriteRendererImpl */
#include "../../Element/Public/object.h"            /*! ::opgs16::element::Object */
#include "../../Manager/Public/setting_manager.h"   /*! ::opgs16::manager::SettingManager */

namespace opgs16 {
namespace component {

Sprite2DRenderer::Sprite2DRenderer(element::Object& bind_object,
                                   const std::string& sprite_tag,
                                   const std::string& shader_tag,
                                   const opgs16::resource::Texture2D::IndexSize& texture_index,
                                   const unsigned layer) :
    Component{ bind_object },
    m_impl { new _internal::SpriteRendererImpl(sprite_tag, shader_tag, texture_index, layer) } {
}

ShaderWrapper& Sprite2DRenderer::Wrapper() const {
    return m_impl->Wrapper();
}

void Sprite2DRenderer::SetTexture(const std::string& texture_name) {
    return m_impl->SetTexture(texture_name);
}

const Sprite2DRenderer::IndexSize& Sprite2DRenderer::TextureIndex() const noexcept {
    return m_impl->TextureIndex();
}

// ReSharper disable CppMemberFunctionMayBeConst
void Sprite2DRenderer::SetTextureIndex(const IndexSize& new_index) {
    // ReSharper restore CppMemberFunctionMayBeConst
    m_impl->SetTextureIndex(new_index);
}

void Sprite2DRenderer::SetRenderLayer(const std::string& layer_name) {
    auto& layer_list = manager::SettingManager::Instance().RenderingLayerNameList();
    decltype(layer_list.size()) i = 0;
    for (; i < layer_list.size(); ++i) {
        if (layer_name == layer_list[i]) {
            m_render_layer_index = i;
            break;
        }
    }

    if (i == layer_list.size()) m_render_layer_index = 0;
}

void Sprite2DRenderer::SetRenderLayer(const size_t layer_index) {
    const auto list_size = manager::SettingManager::Instance().RenderingLayerNameList().size();
    m_render_layer_index = (layer_index >= list_size) ? 0 : layer_index;
}

std::string Sprite2DRenderer::RenderLayerNameOf() const {
    return manager::SettingManager::Instance().RenderingLayerName(m_render_layer_index);
}

// ReSharper disable CppMemberFunctionMayBeConst
void Sprite2DRenderer::RenderSprite() {
    // ReSharper restore CppMemberFunctionMayBeConst
    m_impl->RenderSprite();
}

Sprite2DRenderer::~Sprite2DRenderer() = default;

} /*! opgs16::component */
} /*! opgs16 */

