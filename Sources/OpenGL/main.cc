#include "main.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "camera.h"

auto g_delta_time   = 0.f;
auto g_last_frame   = 0.f;

GLFWwindow* Init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, camera::MouseControl);

    glewInit();
    return window;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void RefreshDeltaTime(const float current_time) {
    g_delta_time = current_time - g_last_frame;
    g_last_frame = current_time;
}

bool DoesKeyPressed(GLFWwindow* const window, const int keycode) {
    if (glfwGetKey(window, keycode) == GLFW_PRESS)
        return true;
    else
        return false;
}

void ProcessInput(GLFWwindow* const window) {
    if (DoesKeyPressed(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    else {
        float camera_speed = 2.5f * g_delta_time;
        auto& camera = camera::GetCamera();

        if (DoesKeyPressed(window, GLFW_KEY_W))
            camera.MovePosition(camera::Movement::UP, camera_speed);
        else if (DoesKeyPressed(window, GLFW_KEY_S))
            camera.MovePosition(camera::Movement::DOWN, camera_speed);
        else if (DoesKeyPressed(window, GLFW_KEY_A))
            camera.MovePosition(camera::Movement::LEFT, camera_speed);
        else if (DoesKeyPressed(window, GLFW_KEY_D))
            camera.MovePosition(camera::Movement::RIGHT, camera_speed);
    }
}



