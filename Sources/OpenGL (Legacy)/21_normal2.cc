#include <cstdarg>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "stb_image.h"

#include "camera.h"
#include "constant.h"
#include "helper.h"
#include "main.h"
#include "Model.h"
#include "shader.h"

#include <type_traits>
using helper::BindingObject;

auto& InitCamera() {
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);

    return camera;
}

int main() {
    // Initialize Window
    auto window = Init();

    // Configure global OpenGL state
    // -----------
    glEnable(GL_DEPTH_TEST);

    helper::Shader shader{ "21_shaders/normal.vert", "21_shaders/normal.frag" };
    auto object = helper::CreateBindingObject(quad_21, 5, { {0, 2, 0}, {1, 3, 2 } });

    auto texture = helper::Texture2D("wall.jpg", GL_RGB);

    // Make offset[100], each item store glm::vec2(x, y) to draw.
    std::array<glm::vec2, 100> translations;
    size_t index = 0;
    const float offset = 0.1f;
    for (auto y = -10; y < 10; y += 2) {
        for (auto x = -10; x < 10; x += 2) {
            glm::vec2&& translation{ (float)x / 10.0f + offset, (float)y / 10.0f + offset };
            translations[index] = translation;
            ++index;
        }
    }

    glBindVertexArray(object.vao);
    GLuint instance_vbo;
    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * translations.size(), 
                 &translations[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set Camera
    auto& camera = InitCamera();
    glm::mat4 m_view = camera.GetViewMatrix();

    // ************************************************************************
    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Rendering
        // *********
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(object.vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, translations.size());

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime((float)glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glfwTerminate();
    return 0;
}