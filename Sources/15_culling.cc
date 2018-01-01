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

int main() {
    // Initialize Window
    auto window = Init();
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    // Bind vertex data to be used into Vertex Buffer Object 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    // Generate and bind VAO is used for emitting light;
    GLuint VAO; {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    helper::Texture2D texture_floor{ "15_textures/checker.jpg", GL_RGB };
    texture_floor.SetTextureParameterI({ 
        std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE),
        std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE),
        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_NEAREST),
        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_NEAREST) });

    // ************************************************************************
    // Set Camera
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);

    // Make shader program with vs and fs path.
    helper::Shader shader_object{"14_shaders/normal.vert"   , "14_shaders/window.frag"};

    glm::mat4 m_view = camera.GetViewMatrix();

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Rendering
        glClearColor(.05f, .05f, .05f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_view = camera.GetViewMatrix();

        // use model shader object
        shader_object.Use();
        shader_object.SetVecMatirix4f("view"    , m_view);
        shader_object.SetVecMatirix4f("projection", camera.GetProjection());

        {
            glm::mat4 model;
            shader_object.SetVecMatirix4f("model", model);
            shader_object.SetInt("texture_diffuse1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_floor.GetId());

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime((float)glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glfwTerminate();
    return 0;
}
