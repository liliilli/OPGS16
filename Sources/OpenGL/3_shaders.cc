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

    // Data Setting
    float vertex_data[] = {
        -.5f,   -.5f,   +.0f,
        +.0f,   -.5f,   +.0f,
        -.25f,  +.5f,   +.0f,
    };

    float rainbow_vertex_data[] = {
        // Positions            // Colors
        -.5f,   -.5f,   +.0f,   1.0f,   0.0f,   0.0f,
        +.0f,   -.5f,   +.0f,   0.0f,   1.0f,   0.0f,
        -.25f,  +.5f,   +.0f,   0.0f,   0.0f,   1.0f
    };

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VBO, VAO;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    
    // Set up rainbow vbo, vao and etc.
    GLuint rainbow_vbo, rainbow_vao;
    {
        glGenVertexArrays(1, &rainbow_vao);
        glBindVertexArray(rainbow_vao);

        glGenBuffers(1, &rainbow_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, rainbow_vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(rainbow_vertex_data), 
                     rainbow_vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                              6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                              6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<Shader>(
        "vertex_shader.vert", "fragment_shader_3.frag");
    auto rainbow_shader = std::make_unique<Shader>(
        "vertex_shader_2.vert", "fragment_shader_4.frag");

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            float time_value = glfwGetTime();
            float green_value = (sin(time_value) / 2.f) + .5f;
            int vertex_color_location = 
                glGetUniformLocation(shader_program->getId(), "outColor");

            shader_program->Use();
            glUniform4f(vertex_color_location, 0.f, green_value, 0.f, 1.f);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        {
            rainbow_shader->Use();
            rainbow_shader->SetFloat("x_offset", 0.5f);

            glBindVertexArray(rainbow_vao);
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