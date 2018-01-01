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

// Camera Instance
class Camera {
public:

    const glm::mat4 GetViewMatrix() const;

    void Refresh();
    void SetValues(const float x_offset, const float y_offset);
    void MovePosition(Movement mode, const float delta);

private:
    const float c_SENSITIVITY   = 0.05f;
    const glm::vec3 c_UP_AXIS   = glm::vec3(0, 1, 0);

    glm::vec3 m_position        = glm::vec3(0, 0, 3);
    glm::vec3 m_front           = glm::vec3(0, 0,-1);

    float m_yaw         = 0.f;
    float m_pitch       = 0.f;
};

/**
 * 
 */
Camera& GetCamera();

}

#endif