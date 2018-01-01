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

#include <type_traits>
using helper::BindingObject;

auto& InitCamera() {
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);

    return camera;
}

GLuint LoadCubemap() {
    const std::array<std::string, 6> cubemap_paths = { 
        "right", "left", "top", "bottom", "back", "front" };

    // Create a cubemap
    GLuint texture_cubemap;
    glGenTextures(1, &texture_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

    int width, height, channels;
    unsigned char* data;
    for (auto i = 0; i < cubemap_paths.size(); ++i) {
        data = stbi_load(("17_textures/skybox/" + cubemap_paths[i] + ".jpg").c_str(),
                         &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture_cubemap;
}

int main() {
    // Initialize Window
    auto window = Init();

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Make Texture container.
    helper::Texture2D texture_floor{ "15_textures/checker_2.png", GL_RGBA };
    texture_floor.SetTextureParameterI({
        {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
        {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
        {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
        {GL_TEXTURE_MAG_FILTER, GL_NEAREST} });
    helper::Texture2D texture_smile{ "15_textures/smile.png", GL_RGBA };
    GLuint texture_cubemap = LoadCubemap();
    
    model::Model nanosuit{ "17_textures/nanosuit/nanosuit.obj" };

    // Make shader program with vs and fs path.
    helper::Shader shader_model{ "17_shaders/normal.vert", "17_shaders/normal.frag" };
    helper::Shader shader_cube{ "17_shaders/cube.vert", "17_shaders/cube.frag" };
    helper::Shader shader_box{ "14_shaders/normal.vert", "14_shaders/window.frag" };
    helper::Shader shader_object{ "16_shaders/normal.vert", "14_shaders/window.frag" };
    helper::Shader shader_rotation{ "16_shaders/rotation.vert", "14_shaders/window.frag" };
    helper::Shader reflection{ "17_shaders/reflection.vert", "17_shaders/reflection.frag" };
    helper::Shader refraction{ "17_shaders/reflection.vert", "17_shaders/refraction.frag" };
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/gaussian.frag"};
    helper::Shader shader_screen{ "16_shaders/retrieve.vert" , "16_shaders/retrieve.frag" };
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/edge.frag"};
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/grayscale.frag"};

    BindingObject sky   = helper::CreateBindingObject(skyboxVertices, 3, { {0, 3, 0} });
    BindingObject quad  = helper::CreateBindingObjectEBO(quad_info, 8, 
                                                         { {0, 3, 0}, {1, 2, 6} }, quad_indices);

    BindingObject box   = helper::CreateBindingObject(box_data, 5, { {0, 3, 0}, {2, 2, 3} });
    BindingObject reflection_box = helper::CreateBindingObject(complete_box, 8, 
                                                               { {0, 3, 0}, {1, 3, 3} });

    // Set box surface framebuffer
    GLuint box_fb;
    glGenFramebuffers(1, &box_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, box_fb);
    GLuint box_texture_cb; {
        glGenTextures(1, &box_texture_cb);
        glBindTexture(GL_TEXTURE_2D, box_texture_cb);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, box_texture_cb, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

        glBindFramebuffer(GL_FRAMEBUFFER, box_fb);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        {
            shader_object.Use(); {
                shader_object.SetInt("texture_diffuse1", 0);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_floor.GetId());

                glBindVertexArray(quad.vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            
            shader_rotation.Use(); {
                shader_rotation.SetInt("texture_diffuse1", 0);

                glm::mat4 model;
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 0, 1));
                shader_rotation.SetVecMatirix4f("rotation", model);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_smile.GetId());

                glBindVertexArray(quad.vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
       
        // First pass
        // Draw objects on user-defined frame buffer and write Color values on buffer.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.05f, .05f, .05f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_view = camera.GetViewMatrix();

        glDepthFunc(GL_LEQUAL);
        shader_cube.Use(); {
            shader_cube.SetVecMatirix4f("view", m_view);
            shader_cube.SetVecMatirix4f("projection", camera.GetProjection());

            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);
            glBindVertexArray(sky.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        glDepthFunc(GL_LESS);
        shader_box.Use(); {
            shader_box.SetVecMatirix4f("view", m_view);
            shader_box.SetVecMatirix4f("projection", camera.GetProjection());
            shader_box.SetInt("texture_diffuse1", 0);

            {
                glm::mat4 model;
                shader_box.SetVecMatirix4f("model", model);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, box_texture_cb);

                glBindVertexArray(box.vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }

        reflection.Use(); {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(2, 0, 0));

            reflection.SetVecMatirix4f("model", model);
            reflection.SetVecMatirix4f("view", m_view);
            reflection.SetVecMatirix4f("projection", camera.GetProjection());
            
            reflection.SetVec3f("cameraPos", camera.GetPosition());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

            glBindVertexArray(reflection_box.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        refraction.Use(); {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(4, 0, 0));

            refraction.SetVecMatirix4f("model", model);
            refraction.SetVecMatirix4f("view", m_view);
            refraction.SetVecMatirix4f("projection", camera.GetProjection());

            refraction.SetVec3f("cameraPos", camera.GetPosition());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

            glBindVertexArray(reflection_box.vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        shader_model.Use(); {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(0, 0, -4));
            model = glm::scale(model, glm::vec3(0.2f));

            shader_model.SetVecMatirix4f("model", model);
            shader_model.SetVecMatirix4f("view", m_view);
            shader_model.SetVecMatirix4f("projection", camera.GetProjection());
            shader_model.SetVec3f("cameraPos", camera.GetPosition());

            shader_model.SetInt("texture_diffuse1", 0);
            shader_model.SetInt("texture_specular1", 1);
            shader_model.SetInt("texture_reflection1", 2);
            shader_model.SetInt("skybox", 3);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

            nanosuit.Draw(shader_model);
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