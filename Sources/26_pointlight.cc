#include <iostream>
#include <memory>
#include <functional>
#include "stb_image.h"

#include "camera.h"
#include "constant.h"
#include "helper.h"
#include "main.h"
#include "model.h"
#include "shader.h"
#include "texture.h"

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

    // Quad
    texture::Texture2D tex_hardwood { "25_textures/hardwood.jpg", GL_RGB };
    auto obj_quad = helper::CreateBindingObjectEBO(quad_info, 8, 
                                                   { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} }, 
                                                   quad_indices);

    // Box
    texture::Texture2D tex_container     { "25_textures/container.png", GL_RGBA };
    texture::Texture2D tex_container_spec{ "25_textures/container_spec.png", GL_RGBA };
    std::array<glm::vec3, 3> box_positions = {
        glm::vec3{0, 0.5f, 0},
        glm::vec3{-2.0, 2.2f, -2.5f},
        glm::vec3{1, 0.5f, -4},
    };
    auto obj_normalbox = helper::CreateBindingObject(complete_box, 8,
                                                     { {0, 3, 0}, {1, 3, 3}, {2, 2, 6} });
    // Sphere
    model::Model model_sphere { "22_model/planet/planet.obj" };

    // LightBox
    auto obj_lightbox = helper::CreateBindingObject(complete_box, 8, { {0, 3, 0}, {1, 2, 6} });
    light::PointLight pointlight{ glm::vec3(-4.f, 4.f, -4.f), 
        glm::vec3{.2f}, glm::vec3{1}, glm::vec3{1} };
    auto light_matrix = pointlight.GetMatrix();

    // Cubemap
    GLuint tex_cubemap{ texture::LoadCubemap("26_textures/hardwood.jpg") };

    // Set up Custom frame buffers
    helper::CustomFrameBuffer fb_depth{};
    fb_depth.GenBuffer(helper::BufferMode::TEXTURE_CUBE_MAP, "cube");

    // Shaders
    helper::Shader shad_normal  { "26_shaders/shadow.vert", "26_shaders/shadow.frag" };
    shad_normal.Use(); { // Setting
        shad_normal.SetInt("textureDiffuse1", 0);
        shad_normal.SetInt("textureSpecular1", 1);
        shad_normal.SetInt("shadowMap", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, fb_depth.GetBuffer("cube"));
        glActiveTexture(GL_TEXTURE0);

        shad_normal.SetStructPointLight("pointlight", pointlight);
        shad_normal.SetFloat("farPlane", pointlight.GetFarPlane());
    }

    helper::Shader shad_light{
        "26_shaders/light.vert", "26_shaders/light.geom", "26_shaders/light.frag" };
    shad_light.Use(); {
        for (auto i = 0; i < 6; ++i)
            shad_light.SetVecMatirix4f("lightMatrix[" + std::to_string(i) + "]", light_matrix[i]);
        shad_light.SetVec3f("lightPos", pointlight.GetPosition());
        shad_light.SetFloat("farPlane", pointlight.GetFarPlane());
    }

    helper::Shader shad_cube{ "26_shaders/cube.vert", "26_shaders/cube.frag" };

    helper::Shader shad_lightbox{ "26_shaders/lightbox.vert", "26_shaders/lightbox.frag" };

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Rendering
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = camera.GetViewMatrix();

        fb_depth.Bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        shad_light.Use();

        {
            glm::mat4 model;
            model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
            model = glm::scale(model, glm::vec3(10.0f));
            shad_light.SetVecMatirix4f("model", model);

            glBindVertexArray(obj_quad.vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        for (const auto& pos : box_positions) {
            glm::mat4 model;
            model = glm::translate(model, pos);
            shad_light.SetVecMatirix4f("model", model);

            glBindVertexArray(obj_normalbox.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(-3.4f, 2.f, -1));
            model = glm::scale(model, glm::vec3(.5f));
            shad_light.SetVecMatirix4f("model", model);

            model_sphere.Draw(shad_light);
        }

        fb_depth.Unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glDepthFunc(GL_LEQUAL);
        //shad_cube.Use();
        //shad_cube.SetVecMatirix4f("projection", camera.GetProjection());
        //shad_cube.SetVecMatirix4f("view", view);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, fb_depth.GetBuffer("cube"));

        //glBindVertexArray(obj_normalbox.vao);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);
        //glDepthFunc(GL_LESS);

        shad_lightbox.Use(); { // Only use for light box.
            shad_lightbox.SetVecMatirix4f("view", view);
            shad_lightbox.SetVecMatirix4f("projection", camera.GetProjection());

            glm::mat4 model;
            model = glm::translate(model, pointlight.GetPosition());
            model = glm::scale(model, glm::vec3(0.5f));
            shad_lightbox.SetVecMatirix4f("model", model);

            glBindVertexArray(obj_lightbox.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        shad_normal.Use(); { // Setting
            shad_normal.SetVecMatirix4f("view", view);
            shad_normal.SetVecMatirix4f("projection", camera.GetProjection());
            shad_normal.SetVec3f("cameraPos", camera.GetPosition());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_CUBE_MAP, fb_depth.GetBuffer("cube"));
        }

        {
            glm::mat4 model;
            model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
            model = glm::scale(model, glm::vec3(10.0f));
            shad_normal.SetVecMatirix4f("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex_hardwood.GetId());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tex_hardwood.GetId());

            glBindVertexArray(obj_quad.vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        for (const auto& pos : box_positions) {
            glm::mat4 model;
            model = glm::translate(model, pos);
            shad_normal.SetVecMatirix4f("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex_container.GetId());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tex_container_spec.GetId());
            
            glBindVertexArray(obj_normalbox.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(-3.4f, 2.f, -1));
            model = glm::scale(model, glm::vec3(.5f, .5f, .5f));
            shad_normal.SetVecMatirix4f("model", model);

            model_sphere.Draw(shad_normal);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime(static_cast<float>(glfwGetTime()));
        camera.Refresh();
    }

    glfwTerminate();
    return 0;
}