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
 * @file System/Components/Impl/sprite_renderer_impl.cc
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-10 Refactoring. Move implementations to ::opgs16::component::_internal
 * 2018-04-02 std::string to std::wstring for UTF-16 characters.
 * 2018-04-06 Support to CTexture2DAtlas information. Revise SetTextureIndex
 * 2018-04-08 Supporting change of shader on running.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "sprite_renderer_impl.h"   /*! Header file */
#include "../../Frame/constant.h"   /*! std::array<> quad_info
                                      * std::array<> quad_indice */
#include "../../../System/Manager/Public/texture_manager.h" /*! TextureManager */
#include "../../../System/Manager/Public/shader_manager.h"  /*! ShaderManager */
#include "../../../Headers/import_logger.h" /*! import logger in debug mode */

namespace opgs16 {
namespace component {
namespace _internal {

CSpriteRendererImpl::CSpriteRendererImpl(const std::string& sprite_tag, const std::string& shader_tag,
                                         unsigned texture_index) :
    m_sprite{ manager::TextureManager::Instance().GetTexture(sprite_tag) },
    m_vao{ quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices },
    m_texture_fragment_index{ texture_index } {
    /*! Body */
    m_wrapper.SetShader(manager::ShaderManager::Instance().Shader(shader_tag));
    glGenVertexArrays(1, &empty_vao);

    SetTextureIndex(texture_index);
}

void CSpriteRendererImpl::SetTextureIndex(const unsigned new_index) noexcept {
    if (!m_sprite->DoesHasAtlas()) {
        PUSH_LOG_WARN(L"Bound texture does not have atlas information.\nso failed to assign new_index.");
        m_texture_fragment_index = 0;
        m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
        m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
    }
    else {
        m_texture_fragment_index = new_index;

        auto texel_ptr_ld = m_sprite->GetTexelPtr(texture::CTexture2D::ETexelType::LEFT_DOWN, new_index);
        auto texel_ptr_ru = m_sprite->GetTexelPtr(texture::CTexture2D::ETexelType::RIGHT_UP, new_index);
        if (texel_ptr_ld && texel_ptr_ru) {
            m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ texel_ptr_ld[0], texel_ptr_ld[1] });
            m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ texel_ptr_ru[0], texel_ptr_ru[1] });
        }
        else {
            PUSH_LOG_WARN(L"Any getting texel from resource has been failed.\nTexel is assigned to overall region.");
            m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
            m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
        }
    }
}

void CSpriteRendererImpl::SetTexture(const std::string& texture_name) noexcept {
    m_sprite = manager::TextureManager::Instance().GetTexture(texture_name);
    SetTextureIndex(0);
}

void CSpriteRendererImpl::SetShader(const std::string& shader_name) {
    m_wrapper.SetShader(manager::ShaderManager::Instance().Shader(shader_name));
}

void CSpriteRendererImpl::RenderSprite() {
    m_wrapper.UseShader();  /*! the name is incorrect... */
    glBindVertexArray(empty_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sprite->Id());
    glDrawArraysInstancedBaseInstance(m_primitive_mode, 0, 6, m_instance_count, m_base_instance);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */