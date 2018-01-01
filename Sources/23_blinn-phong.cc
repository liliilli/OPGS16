#include <iostream>
#include <memory>
#include <functional>
#include "stb_image.h"

#include "camera.h"
#include "constant.h"
#include "helper.h"
#include "main.h"
#include "shader.h"

#define SIZE_OF(size, type) (void*)(size * sizeof(type))
#define SIZE_NULL (void*)0

auto& InitCamera() {
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);

    return camera;
}

int main() {
    // Initialize Window
    auto window = Init();
    glEnable(GL_DEPTH_TEST);
    auto& camera = InitCamera();

    glm::mat4 m_proj;
    m_proj = glm::perspective(glm::radians(50.f), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT,
                              0.1f, 100.f);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    auto quad_object = helper::CreateBindingObjectEBO(quad_info, 8, 
                                                      { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} },
                                                      quad_indices);
    auto light_box = helper::CreateBindingObject(box_data, 5, { {0, 3, 0}, {1, 2, 3} });

    // Make shader program with vs and fs path.
    helper::Shader shader{ "23_shaders/normal.vert", "23_shaders/phong.frag" };
    //helper::Shader shader_source{ "8_shaders/8_normal.vert", "8_shaders/8_source.frag" };

    helper::Texture2D texture{ "23_textures/hardwood.jpg", GL_RGB };

    auto light_position = glm::vec3(0.0f, 1.0f, 0.0f);
    auto intensity = 3.0f;

    PointLight point_light{
        light_position,
        glm::vec3(1) * 0.2f, glm::vec3(1), glm::vec3(1),
        1.0, 0.09, 0.032
    };

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Rendering
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use(); {
            glm::mat4 view = camera.GetViewMatrix();
            shader.SetVecMatirix4f("view", view);
            shader.SetVecMatirix4f("projection", camera.GetProjection());

            glm::mat4 model;
            model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
            model = glm::scale(model, glm::vec3(10.0f));
            shader.SetVecMatirix4f("model", model);

            shader.SetInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.GetId());

            shader.SetInt("texture_specular1", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture.GetId());

            shader.SetStructPointLight("uPointLight", point_light);
            shader.SetVec3f("cameraPos", camera.GetPosition());
        }

        glBindVertexArray(quad_object.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime(glfwGetTime());
        camera.Refresh();
    }

    glfwTerminate();
    return 0;
}