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
 * @file System/Object/Impl/Private/object_impl.cc
 * @brief Implementation file of object_impl.h
 * @author Jongmin Yun
 * @log
 * 2018-03-05 Add rendering layer member functions.
 */

#include "object_impl.h"                    /*! Header file */
#include <glm/gtc/matrix_transform.hpp>     /*! glm::rotate */
#include "../../Manager/Public/setting_manager.h"  /*! SettingManager */

namespace {
using opgs16::manager::SettingManager;
}

void ObjectImpl::RefreshFinalPosition() const {
    m_final_position = m_local_position + m_parent_from_position + m_world_position;
    m_final_pos_deprecated = false;
}

void ObjectImpl::RefreshRotateMatrix() const {
	m_rotate_matrix = glm::rotate(glm::mat4{}, glm::radians(m_rotation_local_angle), m_rotation_local_factor);
    m_rotation_deprecated = false;
}

void ObjectImpl::RefreshScaleVector() const {
    m_scale_final_vector = m_scale_local_factor * m_scale_local_value;
    m_scale_deprecated = false;
}

const glm::mat4& ObjectImpl::GetModelMatrix() const {
	if (m_model_matrix_deprecated) {
        if (m_final_pos_deprecated) RefreshFinalPosition();
        if (m_rotation_deprecated)  RefreshRotateMatrix();
        if (m_scale_deprecated)     RefreshScaleVector();

        /*! Rotation */
		m_model = m_rotate_matrix;
        /*! Scale */
        m_model[0] *= m_scale_final_vector[0];
        m_model[1] *= m_scale_final_vector[1];
        m_model[2] *= m_scale_final_vector[2];
        /*! Movement */
        m_model[3][0] = m_final_position.x;
        m_model[3][1] = m_final_position.y;
        m_model[3][2] = m_final_position.z;
		m_model_matrix_deprecated = false;
	}

	return m_model;
}

std::string ObjectImpl::GetTagNameOf() const {
    return SettingManager::Instance().GetTagName(m_tag_index);
}

void ObjectImpl::SetRenderLayer(const std::string& layer_name) {
    auto& layer_list = SettingManager::Instance().RenderingLayerNameList();
    decltype(layer_list.size()) i = 0;
    for (; i < layer_list.size(); ++i) {
        if (layer_name == layer_list[i]) {
            m_tag_index = i;
            break;
        }
    }

    if (i == layer_list.size()) m_tag_index = 0;
}

void ObjectImpl::SetRenderLayer(const size_t layer_index) {
    const auto list_size = SettingManager::Instance().RenderingLayerNameList().size();
    m_render_layer_index = (layer_index >= list_size) ? 0 : layer_index;
}

std::string ObjectImpl::RenderLayerNameOf() const {
    return SettingManager::Instance().RenderingLayerName(m_render_layer_index);
}

void ObjectImpl::SetTag(const std::string& tag_name) {
    auto& tag_list = SettingManager::Instance().GetTagNameList();

    decltype(tag_list.size()) i = 0;
    for (; i < tag_list.size(); ++i) {
        if (tag_name == tag_list[i]) {
            m_tag_index = i;
            break;
        }
    }

    if (i == tag_list.size()) m_tag_index = 0;
}

void ObjectImpl::SetTag(const size_t tag_index) {
    const auto list_size = SettingManager::Instance().GetTagNameList().size();

    if (tag_index >= list_size)
        m_tag_index = 0;
    else
        m_tag_index = tag_index;
}