#include "object_impl.h"                    /*! Header file */
#include <glm\gtc\matrix_transform.hpp>     /*! glm::translate, glm::rotate, glm::scale */
#include "..\..\Manager\setting_manager.h"  /*! SettingManager */

/**
* @brief Refresh Translation matrix
*/
void ObjectImpl::RefreshTranslateMatrix() {
	m_translate = glm::translate(glm::mat4{}, m_local_position + m_final_position);
    m_translate_deprecated = false;
}

/**
 * @brief Refresh Rotation matrix
 */
void ObjectImpl::RefreshRotateMatrix() {
	m_rotate = glm::rotate(glm::mat4{}, glm::radians(m_rotation_angle), m_rotation_factor);
    m_rotation_deprecated = false;
}

/**
 * @brief Refresh Scaling matrix
 */
void ObjectImpl::RefreshScaleMatrix() {
	m_scale = glm::scale(glm::mat4{}, m_scale_value * m_scale_factor);
    m_scale_deprecated = false;
}

const glm::mat4 ObjectImpl::GetModelMatrix() {
	if (m_model_matrix_deprecated) {
        if (m_translate_deprecated) RefreshTranslateMatrix();
        if (m_rotation_deprecated) RefreshRotateMatrix();
        if (m_scale_deprecated) RefreshScaleMatrix();

		m_model = glm::mat4{} * m_translate * m_rotate * m_scale;
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