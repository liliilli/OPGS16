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
    glEnable(GL_MULTISAMPLE);

    helper::Shader shader{ "21_shaders/brute.vert", "21_shaders/model.frag" };
    helper::Shader shader_astroid{ "21_shaders/clever_astroid.vert", "21_shaders/model.frag" };

    model::Model rock{ "22_model/rock/rock.obj" };
    model::Model venus{ "22_model/planet/planet.obj" };

    // Make offset[100], each item store glm::vec2(x, y) to draw.
    std::array<glm::mat4, 2000> translations;
    std::random_device seed;
    std::mt19937 engine(seed());
    std::normal_distribution<> random{ 0, 6 };

    float radius { 50.0f };
    const auto amount = translations.size();

    for (auto i = 0u; i < translations.size(); ++i) {
        glm::mat4 model;
        // 1. Translations

        float angle { i / static_cast<float>(translations.size()) * 360.f };
        float displacement { static_cast<float>(random(engine)) };
        float x { sin(angle) * radius + displacement };

        displacement = static_cast<float>(random(engine));
        float y { displacement * 0.4f };

        displacement = static_cast<float>(random(engine));
        //displacement = (engine() % (int)(200 * offset)) / 100.f - offset;
        float z { cos(angle) * radius + displacement };

        model = glm::translate(model, glm::vec3(x, y, z));

        float scale { engine() % 20 / 100.f + 0.05f };
        model = glm::scale(model, glm::vec3(scale));

        float rotation = static_cast<float>(engine() % 360);
        model = glm::rotate(model, rotation, glm::vec3(.4f, .6f, .8f));

        translations[i] = model;
    }

    rock.SetVAOTranslation(translations);

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
        m_view = camera.GetViewMatrix();
        shader.SetVecMatirix4f("view", m_view);
        shader.SetVecMatirix4f("projection", camera.GetProjection());

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f));
        shader.SetVecMatirix4f("model", model);
        venus.Draw(shader);

        shader_astroid.Use();
        shader_astroid.SetVecMatirix4f("view", m_view);
        shader_astroid.SetVecMatirix4f("projection", camera.GetProjection());
        rock.Draw(shader, translations.size());

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime((float)glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glfwTerminate();
    return 0;
}