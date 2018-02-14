#include "object_impl.h"
#include <glm\gtc\matrix_transform.hpp>

/**
* @brief Refresh Translation matrix
*/
void ObjectImpl::RefreshTranslateMatrix() {
	m_translate = glm::translate(glm::mat4{}, m_final_position);
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

