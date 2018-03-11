#ifndef OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
#define OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H

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
 * @file System/Components/Impl/sprite_renderer_impl.h
 * @brief Private implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-28 Change constructor to use texture_index type.
 * 2018-03-10 Refactoring.
 */

#include "../../Frame/vertex_array_object.h"    /*! VertexArrayObject */
#include "../../Headers/Fwd/objectfwd.h"        /*! texture::Texture2D */
#include "../../Manager/Public/resource_type.h" /*! resource::Texture2D::IndexSize */
#include "../../Manager/Public/texture_manager.h"
#include "../../Shader/shader_wrapper.h"        /*! ShaderWrapper */

namespace opgs16 {
namespace component {
namespace _internal {

/*!
 * @class SpriteRendererImpl
 * @brief Pointer to implementation structure.
 *
 * @log
 * 2018-02-28 Change constructor to use texture_index type. and variable store texture index.
 * Add related boilerplate function.
 *
 * @todo Implement rendering layer priority feature.
 */
class SpriteRendererImpl final {
private:
    using IndexSize = opgs16::resource::Texture2D::IndexSize;

public:
    /** Make Sprite2DRenderer instance. (Constructor) */
	SpriteRendererImpl(const std::string& sprite_tag,
                       const std::string& shader_tag,
                       const opgs16::resource::Texture2D::IndexSize& texture_index,
                       unsigned layer);

    inline ShaderWrapper& Wrapper() {
        return m_wrapper;
    }

    inline const IndexSize& TextureIndex() const noexcept {
        return m_index;
    }

    inline void SetTextureIndex(const IndexSize& new_index) noexcept {
        m_index = new_index;
        m_wrapper.ReplaceUniformValue("uTexIndex", glm::vec2{ m_index.x_sep, m_index.y_sep });
    }

    inline void SetTexture(const std::string& texture_name) {
        m_sprite = manager::TextureManager::Instance().GetTexture(texture_name);
        auto [cell_x, cell_y] = m_sprite->CellSize();
        m_wrapper.ReplaceUniformValue("uWHSize", glm::vec2{ cell_x, cell_y });
    }

    void RenderSprite();

private:
	texture::Texture2D* m_sprite;	/** Sprite 2d texture stores image information. */
	VertexArrayObject m_vao;	    /** Quad VAO to render sprite on screen. */
	ShaderWrapper m_wrapper;		/** Shader is in ShaderManager, render sprite. */

    IndexSize m_index;
    GLuint empty_vao;
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
