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
 * @file System/Components/Impl/sprite_renderer_impl.cc
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Refactoring. Move implementations to ::opgs16::component::_internal
 */

#include "sprite_renderer_impl.h"   /*! Header file */
#include "../../Frame/constant.h"   /*! std::array<> quad_info
                                      * std::array<> quad_indice */
#include "../../../System/Manager/Public/texture_manager.h"    /*! TextureManager */
#include "../../../System/Manager/Public/shader_manager.h"      /*! ShaderManager */

namespace opgs16 {
namespace component {
namespace _internal {

SpriteRendererImpl::SpriteRendererImpl(const std::string & sprite_tag,
                                       const std::string & shader_tag,
                                       const opgs16::resource::Texture2D::IndexSize& texture_index,
                                       unsigned layer) :
    m_sprite{ opgs16::manager::TextureManager::Instance().GetTexture(sprite_tag) },
    m_vao{ quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices },
    m_index { texture_index } {
    m_wrapper.SetShader(opgs16::manager::ShaderManager::Instance().Shader(shader_tag));
    glGenVertexArrays(1, &empty_vao);

    auto [cell_x, cell_y] = m_sprite->CellSize();
    m_wrapper.InsertUniformValue("uWHSize", glm::vec2{ cell_x, cell_y });
    m_wrapper.InsertUniformValue("uTexIndex",
                                 glm::vec2{
                                     (float)texture_index.x_sep,
                                     (float)texture_index.y_sep }
    );
}

void SpriteRendererImpl::RenderSprite() {
	m_wrapper.UseShader();  /*! the name is incorrect... */
	glBindVertexArray(empty_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sprite->Id());
    glDrawArrays(GL_TRIANGLES, 0, 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */