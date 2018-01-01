#ifndef OPENGL_TUTORIAL_MAIN_H
#define OPENGL_TUTORIAL_MAIN_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

constexpr auto TITLE = "LearnOpenGL";
constexpr auto SCREEN_WIDTH     = 720u;
constexpr auto SCREEN_HEIGHT    = 480u;

GLFWwindow* Init();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* const window);
void RefreshDeltaTime(const float current_time);

#endif
