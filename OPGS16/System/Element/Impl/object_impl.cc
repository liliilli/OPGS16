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
 * @file System/Object/Impl/Private/object_impl.cc
 * @brief Implementation file of object_impl.h
 * @author Jongmin Yun
 * @log
 * 2018-03-05 Add rendering layer member functions.
 * 2018-03-11 Moved implementation contents into ::opgs16::element::_internal.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "object_impl.h"                    /*! Header file */
#include <glm/gtc/matrix_transform.hpp>     /*! glm::rotate */
#include "../../Manager/Public/setting_manager.h"  /*! MSettingManager */

namespace opgs16 {
namespace element {
namespace _internal {

namespace {
using manager::MSettingManager;
} /*! unnamed namespace */

void CObjectImpl::RefreshFinalPosition() const {
    m_final_position = m_local_position + m_parent_from_position + m_world_position;
    m_final_pos_deprecated = false;
}

void CObjectImpl::RefreshRotateMatrix() const {
	m_local_rotate_matrix = glm::rotate(glm::mat4{}, glm::radians(m_rotation_local_angle), m_rotation_local_factor);
    m_local_rotation_deprecated = false;
}

void CObjectImpl::RefreshParentRotationMatrix() const {
    m_parent_rotate_matrix = glm::rotate(glm::mat4{},
                                         glm::radians(m_rotation_parent_from_angle), m_rotation_parent_from_factor);
    m_parent_rotation_deprecated = false;
}

void CObjectImpl::RefreshWorldRotationMatrix() const {
    m_world_rotate_matrix = glm::rotate(glm::mat4{}, glm::radians(m_rotation_world_angle), m_rotation_world_factor);
    m_world_rotation_deprecated = false;
}

void CObjectImpl::RefreshScaleVector() const {
    m_scale_final_vector = m_scale_local_factor * m_scale_local_value;
    m_scale_deprecated = false;
}

const glm::mat4& CObjectImpl::GetModelMatrix() const {
	if (m_local_model_matrix_deprecated) {
        if (m_final_pos_deprecated)       RefreshFinalPosition();
        if (m_local_rotation_deprecated)  RefreshRotateMatrix();
        if (m_scale_deprecated)           RefreshScaleVector();

        /*! Rotation */
		m_local_model = m_local_rotate_matrix;
        /*! Scale */
        m_local_model[0] *= m_scale_final_vector[0];
        m_local_model[1] *= m_scale_final_vector[1];
        m_local_model[2] *= m_scale_final_vector[2];
        /*! Movement */
        m_local_model[3][0] = m_final_position.x;
        m_local_model[3][1] = m_final_position.y;
        m_local_model[3][2] = m_final_position.z;
		m_local_model_matrix_deprecated = false;
	}

    // Performance...???
    if (m_offset_model_matrix_deprecated) {
        if (m_parent_rotation_deprecated) RefreshParentRotationMatrix();
        if (m_world_rotation_deprecated)  RefreshWorldRotationMatrix();
        m_offset_model_matrix_deprecated = false;
    }

    m_final_model = m_world_rotate_matrix * (m_parent_rotate_matrix * m_local_model);
    m_final_position.x = m_final_model[3][0];
    m_final_position.y = m_final_model[3][1];
    m_final_position.z = m_final_model[3][2];
	return m_final_model;
}

std::string CObjectImpl::GetTagNameOf() const {
    return MSettingManager::Instance().GetTagName(m_tag_index);
}

void CObjectImpl::SetTag(const std::string& tag_name) {
    auto& tag_list = MSettingManager::Instance().GetTagNameList();

    decltype(tag_list.size()) i = 0;
    for (; i < tag_list.size(); ++i) {
        if (tag_name == tag_list[i]) {
            m_tag_index = i;
            break;
        }
    }

    if (i == tag_list.size()) m_tag_index = 0;
}

void CObjectImpl::SetTag(const size_t tag_index) {
    const auto list_size = MSettingManager::Instance().GetTagNameList().size();

    if (tag_index >= list_size)
        m_tag_index = 0;
    else
        m_tag_index = tag_index;
}

} /*! opgs16::element::_internal */
} /*! opgs16::element */
} /*! opgs16 */

