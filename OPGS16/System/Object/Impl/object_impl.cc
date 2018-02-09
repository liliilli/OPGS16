#include "object_impl.h"
#include <glm\gtc\matrix_transform.hpp>

/**
* @brief Refresh Translation matrix
*/
void ObjectImpl::RefreshTranslateMatrix() {
	m_translate = glm::translate(glm::mat4{}, m_local_position + m_local_final_position);
}

/**
 * @brief Refresh Rotation matrix
 */
void ObjectImpl::RefreshRotateMatrix() {
	m_rotate = glm::rotate(glm::mat4{}, glm::radians(m_rotation_angle), m_rotation_factor);
}

/**
 * @brief Refresh Scaling matrix
 */
void ObjectImpl::RefreshScaleMatrix() {
	m_scale = glm::scale(glm::mat4{}, m_scale_value * m_scale_factor);
}

const glm::mat4 ObjectImpl::GetModelMatrix() {
	if (m_is_changed) {
		m_model = glm::mat4{} * m_translate * m_rotate * m_scale;
		m_is_changed = false;
	}

	return m_model;
}

