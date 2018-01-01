
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "helper.h"
#include "stb_image.h"
using helper::Shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

constexpr auto TITLE = "LearnOpenGL";
constexpr auto SCREEN_WIDTH     = 640u;
constexpr auto SCREEN_HEIGHT    = 640u;
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

auto g_min_density = 0.2;
int main() {
    // Initialize Window
    auto window = init();

    // Data Setting
    float vertex_data[] = {
        // Positions            // Colors               // Texture Coords
        -.75f,   -.75f,   +.0f,   1.0f,   0.0f,   0.0f,   0.0f,   0.0f,
        +.75f,   -.75f,   +.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,
        +.75f,   +.75f,   +.0f,   0.0f,   0.0f,   1.0f,   1.0f,   1.0f,
        -.75f,   +.75f,   +.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f
    };

    unsigned indices_data[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VBO, VAO, EBO;
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_data), 
                         indices_data, GL_STATIC_DRAW);
        }

        {
            auto stride = 8 * sizeof(float);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
    }


    // Image load upright, not to be displayed as inverted texture.
    stbi_set_flip_vertically_on_load(true);

    // Generate a texture
    GLuint texture;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Set the texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Load image file to use as texture image.
        auto width = 0;
        auto height = 0;
        auto nr_channels = 0;
        auto data = stbi_load("container.jpg", 
                              &width, &height, &nr_channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
        }
        stbi_image_free(data);
    }

    GLuint texture_2;
    {
        glGenTextures(1, &texture_2);
        glBindTexture(GL_TEXTURE_2D, texture_2);
        // Set the texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Load image file to use as texture image.
        auto width = 0;
        auto height = 0;
        auto nr_channels = 0;
        auto data = stbi_load("awesomeface.png", 
                              &width, &height, &nr_channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                         GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "FAILED::LOAD::TEXTURE" << std::endl;
        }
        stbi_image_free(data);
    }

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<Shader>(
        "4_heroes.vert", 
        "4_mix_min.frag");

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_2);
        }

        shader_program->Use();
        {
            glUniform1i(glGetUniformLocation(shader_program->getId(), "texture1"), 0);
            shader_program->SetInt("texture2", 1);

            auto min_density = glGetUniformLocation(shader_program->getId(), "min_density");
            glUniform1f(min_density, static_cast<float>(g_min_density));
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

bool DoesKeyPressed(GLFWwindow* const window, const int keycode) {
    if (glfwGetKey(window, keycode) == GLFW_PRESS)
        return true;
    else
        return false;
}

void process_input(GLFWwindow* const window) {
    if (DoesKeyPressed(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    else if (DoesKeyPressed(window, GLFW_KEY_UP)) {
        if (g_min_density >= 1.0) g_min_density = 1.0;
        else g_min_density += 0.0001;
    }
    else if (DoesKeyPressed(window, GLFW_KEY_DOWN)) {
        if (g_min_density <= 0.0) g_min_density = 0.0;
        else g_min_density -= 0.0001;
    }
}