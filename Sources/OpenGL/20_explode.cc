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

    helper::Shader shader{ "20_shaders/model.vert", 
        "20_shaders/model_explode.geom", 
        "20_shaders/model_explode.frag" };

    auto object = helper::CreateBindingObject(points, 5, { {0, 2, 0}, {1, 3, 2 } });

    model::Model model = model::Model{ "nanosuit/nanosuit.obj" };

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

        glm::mat4 world;
        world = glm::scale(world, glm::vec3(0.2f));
        shader.SetVecMatirix4f("model", world);

        m_view = camera.GetViewMatrix();
        shader.SetVecMatirix4f("view", m_view);
        shader.SetVecMatirix4f("projection", camera.GetProjection());
        shader.SetInt("texture_diffuse1", 0);
        shader.SetFloat("time", static_cast<float>(glfwGetTime()));

        model.Draw(shader);

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime((float)glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glfwTerminate();
    return 0;
}