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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Object/Impl/object_impl.cc
 * @brief Implementation file of object_impl.h
 * @author Jongmin Yun
 * @log
 * 2018-03-05 Add rendering layer member functions.
 * 2018-03-11 Moved implementation contents into ::opgs16::element::_internal.
 * 2018-04-18 Change function and mechanism of rotation.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\Impl\object_impl.h>   /// Header file

#include <glm\gtc\matrix_transform.hpp> /// glm::rotate
#include <Manager\setting_manager.h>    /// ::opgs16::manager::MSettingManager

namespace opgs16 {
namespace element {
namespace _internal {

namespace {
using manager::MSettingManager;
constexpr float k_2pi{ 2 * glm::pi<float>() };

const static glm::vec3 k_vec3_1{ 1.f };
const static glm::mat4 k_rotation_init{ glm::rotate(glm::mat4{}, k_2pi, k_vec3_1) };

bool IsAllAngleValueZero(const float (&angle)[3]) {
    unsigned zero_count{ 0 };
    for (float i : angle)
        if (std::fabsf(i) <= 0.001f) { ++zero_count; }

    return zero_count >= 3;
}

glm::mat4 GetRotationMatrix(const float (&angle)[3]) {
    glm::mat4 matrix;
    matrix = glm::rotate(matrix, glm::radians(angle[0]), glm::vec3{ 1, 0, 0 });
    matrix = glm::rotate(matrix, glm::radians(angle[1]), glm::vec3{ 0, 1, 0 });
    matrix = glm::rotate(matrix, glm::radians(angle[2]), glm::vec3{ 0, 0, 1 });
    return matrix;
}

} /*! unnamed namespace */

void CObjectImpl::RefreshFinalPosition() const {
    m_final_position = m_local_position + m_parent_from_position;

    for (auto i = 0u; i < 3u; ++i) { // Loop unrolling
        m_final_position[i] += m_wp_rotate_matrix[0][i] * m_world_position[0];
        m_final_position[i] += m_wp_rotate_matrix[1][i] * m_world_position[1];
        m_final_position[i] += m_wp_rotate_matrix[2][i] * m_world_position[2];
    }
}

void CObjectImpl::RefreshRotateMatrix() const {
    if (IsAllAngleValueZero(m_rotation_local_angle_n))
        m_local_rotate_matrix = k_rotation_init;
    else
        m_local_rotate_matrix = GetRotationMatrix(m_rotation_local_angle_n);
}

void CObjectImpl::RefreshWpRotationMatrix() const {
    if (IsAllAngleValueZero(m_rotation_wp_angle_n))
        m_wp_rotate_matrix = k_rotation_init;
    else
        m_wp_rotate_matrix = GetRotationMatrix(m_rotation_wp_angle_n);
}

void CObjectImpl::RefreshRotationWorldParentAngle(const EDirection direction) {
    float* wp_target_angle{ nullptr };
    switch (direction) {
    case EDirection::X:
        wp_target_angle = &m_rotation_wp_angle_n[0];
        *wp_target_angle = m_rotation_parent_angle_n[0] + m_rotation_world_angle_n[0];
        break;
    case EDirection::Y:
        wp_target_angle = &m_rotation_wp_angle_n[1];
        *wp_target_angle = m_rotation_parent_angle_n[1] + m_rotation_world_angle_n[1];
        break;
    case EDirection::Z: wp_target_angle = &m_rotation_wp_angle_n[2];
        *wp_target_angle = m_rotation_parent_angle_n[2] + m_rotation_world_angle_n[2];
        break;
    default: break;
    }

    if (wp_target_angle) {
        const float angle = std::fmodf(*wp_target_angle, 360.f);
        *wp_target_angle = (angle > 180.f) ? angle - 360.f : ((angle <= -180.f) ? angle + 360.f : angle);
    }
}

void CObjectImpl::RefreshScaleVector() const {
    m_scale_final_vector = m_scale_local_factor * m_scale_local_value;
}

const glm::mat4& CObjectImpl::GetModelMatrix() const {
    if (m_offset_model_matrix_deprecated) {
        RefreshWpRotationMatrix();
        RefreshFinalPosition();
        m_offset_model_matrix_deprecated = false;
    }

	if (m_local_model_matrix_deprecated) {
        if (m_final_pos_deprecated) {
            RefreshFinalPosition();
            m_final_pos_deprecated = false;
        }

        if (m_local_rotation_deprecated) {
            RefreshRotateMatrix();
            m_local_rotation_deprecated = false;
        }

        if (m_scale_deprecated) {
            RefreshScaleVector();
            m_scale_deprecated = false;
        }

		m_local_model_matrix_deprecated = false;
	}

    m_final_model = m_wp_rotate_matrix * m_local_rotate_matrix;
    m_final_model[0] *= m_scale_final_vector[0];
    m_final_model[1] *= m_scale_final_vector[1];
    m_final_model[2] *= m_scale_final_vector[2];
    m_final_model[3][0] = m_final_position.x;
    m_final_model[3][1] = m_final_position.y;
    m_final_model[3][2] = m_final_position.z;

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

