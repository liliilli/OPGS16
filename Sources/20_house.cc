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

    helper::Shader shader{ "20_shaders/normal.vert", 
        "20_shaders/house.geom", 
        "20_shaders/house.frag" };

    auto object = helper::CreateBindingObject(points, 5, { {0, 2, 0}, {1, 3, 2 } });

    auto model = model::Model{ "nanosuit/nanosuit.obj" };

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
        glDrawArrays(GL_POINTS, 0, 4);

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime((float)glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glfwTerminate();
    return 0;
}