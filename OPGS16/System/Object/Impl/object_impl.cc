#include "object_impl.h"                    /*! Header file */
#include <glm\gtc\matrix_transform.hpp>     /*! glm::rotate */
#include "..\..\Manager\setting_manager.h"  /*! SettingManager */

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
    return SettingManager::GetInstance().GetTagName(m_tag_index);
}

void ObjectImpl::SetTag(const std::string tag_name) {
    auto& tag_list = SettingManager::GetInstance().GetTagNameList();

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
    auto list_size = SettingManager::GetInstance().GetTagNameList().size();

    if (tag_index >= list_size)
        m_tag_index = 0;
    else
        m_tag_index = tag_index;
}