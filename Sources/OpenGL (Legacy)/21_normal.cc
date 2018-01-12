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
        for (auto i = 0; i < translations.size(); ++i) {
            auto location = glGetUniformLocation(
                shader.GetId(), ("offsets[" + std::to_string(i) + "]").c_str());

            glUniform2fv(location, 1, glm::value_ptr(translations[i]));
        }
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