
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helper.h"
using helper::Shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

constexpr auto TITLE = "LearnOpenGL";
constexpr auto SCREEN_WIDTH     = 640u;
constexpr auto SCREEN_HEIGHT    = 480u;
constexpr auto LOG_SIZE         = 0x200u;

GLFWwindow* init() {
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    auto __ = glewInit();

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize Window
    auto window = init();

    float triangle_data[] = {
        -.5f,   -.5f,   +.0f,   1.0f,   0.0f,   0.0f,
        +.5f,   -.5f,   +.0f,   0.0f,   1.0f,   0.0f,
        -.0f,   +.5f,   +.0f,   0.0f,   0.0f,   1.0f
    };

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VBO, VAO;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

        auto stride = 6 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<Shader>(
        "3_rotation.vert", "fragment_shader_4.frag");

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            float time_value = glfwGetTime();
            int vertex_color_location = glGetUniformLocation(shader_program->getId(), "angle");

            shader_program->Use();
            glUniform2f(vertex_color_location, cos(time_value), sin(time_value));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* const window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}