
#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "helper.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* const window);

constexpr auto TITLE = "LearnOpenGL";
constexpr auto SCREEN_WIDTH     = 640u;
constexpr auto SCREEN_HEIGHT    = 640u;

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
        -.75f,   -.75f,   +.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,
        +.75f,   -.75f,   +.0f,   1.0f,   1.0f,   1.0f,   1.0f,   0.0f,
        +.75f,   +.75f,   +.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,
        -.75f,   +.75f,   +.0f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f
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
    auto texture_1 = std::make_unique<helper::Texture2D>("0116_1.png", GL_RGB);
    auto texture_2 = std::make_unique<helper::Texture2D>("awesomeface.png", GL_RGBA);
    texture_1->SetTextureParameterI(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<helper::Shader>(
        "5_transform.vert", "5_frag.frag");

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_1->GetId());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_2->GetId());
        }

        shader_program->Use();

        glUniform1i(glGetUniformLocation(shader_program->GetId(), "texture1"), 0);
        shader_program->SetInt("texture2", 1);

        auto min_density = glGetUniformLocation(shader_program->GetId(), "min_density");
        glUniform1f(min_density, static_cast<float>(g_min_density));
        auto loc_trans = glGetUniformLocation(shader_program->GetId(), "transform");

        {
            glm::mat4 transformation;
            // Reverse order. (translate -> rotate -> scale)
            //transformation = glm::rotate(transformation, 
            //glm::radians(90.f), glm::vec3(0, 0, 1));
            transformation = glm::rotate(transformation, 
                                         static_cast<float>(glfwGetTime()), 
                                         glm::vec3(0, 0, 1));
            transformation = glm::scale(transformation, glm::vec3(.5, .5f, .5f));
            transformation = glm::translate(transformation, glm::vec3(1.f, 1.f, 0.f));

            glUniformMatrix4fv(loc_trans, 1, GL_FALSE, glm::value_ptr(transformation));
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        {
            glm::mat4 transformation;
            // Reverse order. (translate -> rotate -> scale)
            transformation = glm::rotate(transformation, 
                                         static_cast<float>(glfwGetTime()) * 2, 
                                         glm::vec3(0, 0, 1));
            auto scale_amount = sin(glfwGetTime()) * .5f + .5f;
            transformation = glm::scale(transformation, 
                                        glm::vec3(scale_amount, scale_amount, scale_amount));
            transformation = glm::translate(transformation, glm::vec3(-1.f, -1.f, 0.f));

            glUniformMatrix4fv(loc_trans, 1, GL_FALSE, glm::value_ptr(transformation));
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