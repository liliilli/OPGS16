#include "camera.h"
#include <iostream>
#include <glm\glm.hpp>

namespace camera {

// The position used for mouse camera control
bool  c_IS_FIRST        = true;
float m_last_cursor_x   = 0.f;
float m_last_cursor_y   = 0.f;

Camera& GetCamera() {
    static Camera camera{};
    return camera;
}

void SetCursor(const float x_center, const float y_center) {
    m_last_cursor_x = x_center;
    m_last_cursor_y = y_center;
}

void MouseControl(GLFWwindow* window, double x_pos, double y_pos) {
    if (c_IS_FIRST) {
        m_last_cursor_x = (float)x_pos;
        m_last_cursor_y = (float)y_pos;
        c_IS_FIRST = false;
    }

    float x_offset = static_cast<float>(x_pos - m_last_cursor_x);
    float y_offset = static_cast<float>(y_pos - m_last_cursor_y);

    GetCamera().SetValues(x_offset, y_offset);

    m_last_cursor_x = (float)x_pos;
    m_last_cursor_y = (float)y_pos;
}

/**
 * Camera Instance
 */
const glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, k_UP_AXIS);
}

void Camera::Refresh() {
    m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_front   = glm::normalize(m_front);
}

void Camera::SetValues(const float x_offset, const float y_offset) {
    m_yaw   += (x_offset * k_SENSITIVITY);
    m_pitch += (y_offset * k_SENSITIVITY);

    if (m_pitch > 89.f)     m_pitch = 89.f;
    if (m_pitch < -89.f)    m_pitch = -89.f;
}

void Camera::MovePosition(Movement mode, const float delta) {
    switch (mode) {
    case Movement::UP:
        m_position += delta * m_front;
        break;
    case Movement::DOWN:
        m_position -= delta * m_front;
        break;
    case Movement::LEFT:
        m_position -= delta * glm::normalize(glm::cross(m_front, k_UP_AXIS));
        break;
    case Movement::RIGHT:
        m_position += delta * glm::normalize(glm::cross(m_front, k_UP_AXIS));
        break;
    }
}

void Camera::Zoom(ZoomMode mode) {
    switch (mode) {
    case ZoomMode::IN:
        if ((m_projection_angle = m_projection_angle - k_SENSITIVITY) < k_ZOOM_IN_LIMIT)
            m_projection_angle = k_ZOOM_IN_LIMIT;
        break;
    case ZoomMode::OUT:
        if ((m_projection_angle = m_projection_angle + k_SENSITIVITY) > k_ZOOM_OUT_LIMIT)
            m_projection_angle = k_ZOOM_OUT_LIMIT;
        break;
    }

    SetPerspective(m_projection_angle, m_ratio, m_near, m_far);
}

void Camera::SetPerspective(const float sight_degree, const float screen_ratio,
                            const float near, const float far) {
    m_proj = glm::perspective(glm::radians(sight_degree), screen_ratio, near, far);
    m_projection_angle = sight_degree;
    m_ratio = screen_ratio;
    m_near = near;
    m_far = far;
}

}
