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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Components/Impl/sprite_renderer_impl.h
 * @brief Private implementation file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-28 Change constructor to use texture_index type.
 * 2018-03-10 Refactoring.
 * 2018-04-08 Supporting change of shader on running.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../Frame/vertex_array_object.h"    /*! VertexArrayObject */
#include "../../Headers/Fwd/objectfwd.h"        /*! texture::CTexture2D */
#include "../../Manager/Public/resource_type.h" /*! resource::STexture2D::IndexSize */
#include "../../Manager/Public/texture_manager.h"
#include "../../Shader/shader_wrapper.h"        /*! ShaderWrapper */

namespace opgs16 {
namespace component {
namespace _internal {

/*!
 * @class CSpriteRendererImpl
 * @brief Pointer to implementation structure.
 *
 * @log
 * 2018-02-28 Change constructor to use texture_index type. and variable store texture index.
 * Add related boilerplate function.
 * 2018-04-02 std::string to std::wstring for Unicode
 * 2018-04-08 Supporting change of shader on running.
 */
class CSpriteRendererImpl final {
public:
    /** Make Sprite2DRenderer instance. (Constructor) */
	CSpriteRendererImpl(const std::string& sprite_tag, const std::string& shader_tag, unsigned texture_index);

    ShaderWrapper& Wrapper() {
        return m_wrapper;
    }

    /*! Get texture fragment index. if this does not handle atlas, return 0 */
    const unsigned TextureIndex() const noexcept {
        return m_texture_fragment_index;
    }

    /*!
     * @brief Set texture fragment index with new_index value.
     * @param[in] new_index
     */
    void SetTextureIndex(const unsigned new_index) noexcept;

    /*!*/
    void SetTexture(const std::string& texture_name) noexcept;

    /*!*/
    void SetShader(const std::string& shader_name);

    /*!*/
    void RenderSprite();

    /*!*/
    void SetInstanceCount(unsigned instance_count) {
        m_instance_count = instance_count;
    }

private:
	texture::CTexture2D* m_sprite;	/*! Sprite 2d texture stores image information. */
	VertexArrayObject m_vao;	    /*! Quad VAO to render sprite on screen. */
	ShaderWrapper m_wrapper;		/*! Shader is in ShaderManager, render sprite. */

    unsigned m_texture_fragment_index;
    GLuint empty_vao;

    GLenum      m_primitive_mode{ GL_TRIANGLES };
    unsigned    m_instance_count{ 1 };
    unsigned    m_base_instance{ 0 };
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_OBJECT_IMPL_SPRITE_RENDERER_IMPL_H
