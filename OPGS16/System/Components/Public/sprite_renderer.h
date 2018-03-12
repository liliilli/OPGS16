#ifndef OPGS16_SYSTEM_COMPONENTS_SPRITE_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_SPRITE_RENDERER_H

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
 * @file System\Object\sprite_renderer.h
 * @brief The file contains sprite renderer class members.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-26 Componentization of Sprite2DRenderer.
 * 2018-02-28 Add Get/SetTextureIndex() member function.
 * 2018-03-07 Move file to /Component.
 */

#include <string>
#include <memory>

#include "../Internal/component.h"          /*! opgs16::component::_internal::CComponent */
#include "../Internal/component_macro.h"    /*! SET_UP_TYPE_MEMBER() */
#include "../../Headers/Fwd/objectfwd.h"    /*! CSpriteRendererImpl
                                              * ShaderWraper */
#include "../../Manager/Public/object_manager.h"    /*! opgs16::manager::MObjectManager */
#include "../../Manager/Public/resource_type.h"     /*! resource::STexture2D::IndexSize */

namespace opgs16 {
namespace component {

/*!
 * @class CSprite2DRenderer
 * @brief The class for object has to render 2d rendering sprite.
 * SpriteRender class has sprite to render on object position,
 * and has at least one more shader to display to screen.
 *
 * @log
 * 2018-02-26 Componentization of Sprite2DRenderer.
 * 2018-02-28 Add Get/SetTextureIndex() member function.
 * 2018-03-07 Move to opgs16::component namespace.
 */
class CSprite2DRenderer final : public _internal::CComponent {
private:
    using IndexSize = opgs16::resource::STexture2D::IndexSize;
    using pimpl_type = std::unique_ptr<_internal::CSpriteRendererImpl>;

public:
	/*! Make Sprite2DRenderer instance. (Constructor) */
	CSprite2DRenderer(element::CObject& bind_object,
                     const std::string& sprite_tag,
                     const std::string& shader_tag,
                     const IndexSize& texture_index = {0, 0},
                     const unsigned layer = 0);

	/*! Get ShaderWrapper instance. */
	ShaderWrapper& Wrapper() const;

    void SetTexture(const std::string& texture_name);

    void SetTextureIndex(const IndexSize& new_index);

    void SetRenderLayer(const std::string& layer_name);

    void SetRenderLayer(const size_t layer_index);

    /*! Get Texture index position. */
    const IndexSize& TextureIndex() const noexcept;

    inline size_t RenderLayerIndexOf() const noexcept {
        return m_render_layer_index;
    }

    std::string RenderLayerNameOf() const;

    virtual void Update() override final {
        opgs16::manager::MObjectManager::Instance().InsertRenderingObject(&GetObject(),
                                                                         m_render_layer_index);
    };

	/**
	 * @brief Render sprite on screen. Procedure is below.
	 * 1. m_shader is enable (must be enabled), active shader to use.
	 * 2. update shader uniform parameter.
	 * 3. bind texture, render it with final position(PVM) of bound object.
	 */
	void RenderSprite();

    ~CSprite2DRenderer();

private:
    pimpl_type m_impl{};
    size_t m_render_layer_index{ 0 };       /*! Rendering layer index */

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CSprite2DRenderer)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_COMPONENTS_SPRITE_RENDERER_H */