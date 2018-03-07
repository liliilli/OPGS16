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

#include "../_macro.h"
#include "../Internal/component.h"
#include "../../Headers/Fwd/objectfwd.h"    /*! SpriteRendererImpl
                                              * SpriteRendererImplDeleter
                                              * ShaderWraper */
#include "../../Manager/Public/resource_type.h" /*! resource::Texture2D::IndexSize */

namespace opgs16 {
namespace component {

/*!
 * @class Sprite2DRenderer
 * @brief The class for object has to render 2d rendering sprite.
 * SpriteRender class has sprite to render on object position,
 * and has at least one more shader to display to screen.
 *
 * @log
 * 2018-02-26 Componentization of Sprite2DRenderer.
 * 2018-02-28 Add Get/SetTextureIndex() member function.
 * 2018-03-07 Move to opgs16::component namespace.
 */
class Sprite2DRenderer final {
private:
    using IndexSize = opgs16::resource::Texture2D::IndexSize;

public:
	/*! Make Sprite2DRenderer instance. (Constructor) */
	Sprite2DRenderer(const std::string& sprite_tag,
				   const std::string& shader_tag,
                   const opgs16::resource::Texture2D::IndexSize& texture_index = {0, 0},
				   const unsigned layer = 0);

	/*!
	 * @brief Change layer number of this instance.
	 * @param[in] layer Layer number.
	 */
	void SetLayer(const unsigned layer);

	/*!
	 * @brief Get layer number of bound instance.
	 * @return layer number value.
	 */
    unsigned GetLayer() const;

	/*! Get ShaderWrapper instance.  */
	ShaderWrapper& GetWrapper() const;

    const IndexSize& GetTextureIndex() const noexcept ;

    void SetTextureIndex(const IndexSize& new_index);

	/**
	 * @brief Render sprite on screen. Procedure is below.
	 * 1. m_shader is enable (must be enabled), active shader to use.
	 * 2. update shader uniform parameter.
	 * 3. bind texture, render it with final position(PVM) of bound object.
	 */
	void RenderSprite();

    ~Sprite2DRenderer();

private:
    using pimpl_type = std::unique_ptr<SpriteRendererImpl>;
    pimpl_type m_impl{ nullptr };
};

} /*! opgs16::component */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_COMPONENTS_SPRITE_RENDERER_H */