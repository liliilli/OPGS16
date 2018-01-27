#ifndef OPENGL_TUTORIAL_CAMERA_H
#define OPENGL_TUTORIAL_CAMERA_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace camera {

// Global Functions
void SetCursor(const float x_center, const float y_center);
void MouseControl(GLFWwindow* window, double x_pos, double y_pos);

// 
enum class Movement {
    UP, LEFT, DOWN, RIGHT
};

enum class ZoomMode {
    IN, OUT
};

// Camera Instance
class Camera {
public:

    const glm::mat4 GetViewMatrix() const;

    void Refresh();
    void SetValues(const float x_offset, const float y_offset);
    void MovePosition(Movement mode, const float delta);
    void Zoom(ZoomMode mode);
    void SetPerspective(const float sight_degree, const float screen_ratio,
                        const float near, const float far);

    auto GetLocalPosition() const -> const glm::vec3{ return m_position; };
    auto GetProjection() -> glm::mat4& { return m_proj; }

private:
    const float k_SENSITIVITY   = 0.05f;
    const glm::vec3 k_UP_AXIS   = glm::vec3(0, 1, 0);
    const float k_ZOOM_OUT_LIMIT = 90.f;
    const float k_ZOOM_IN_LIMIT = 30.f;

    glm::vec3 m_position        = glm::vec3(0, 3, 1);
    glm::vec3 m_front           = glm::vec3(0, 0,-3);
    glm::mat4 m_proj;
    float m_projection_angle;
    float m_ratio;
    float m_near;
    float m_far;

    float m_yaw         = 0.f;
    float m_pitch       = 0.f;
};

/**
 * 
 */
Camera& GetCamera();

}

#endif