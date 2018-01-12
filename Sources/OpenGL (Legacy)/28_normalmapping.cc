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

auto CreateQuad() {
    // positions
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3(1.0f, -1.0f, 0.0f);
    glm::vec3 pos4(1.0f, 1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1{
        f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
        f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
        f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z) };

    glm::vec3 bitangent1{
        f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
        f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
        f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z) };

    // triangle 2
    // ----------
    glm::vec3 edge12 = pos3 - pos1;
    glm::vec3 edge22 = pos4 - pos1;
    glm::vec2 deltaUV12 = uv3 - uv1;
    glm::vec2 deltaUV22 = uv4 - uv1;

    GLfloat f2 = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent2{
        f2 * (deltaUV22.y * edge12.x - deltaUV12.y * edge22.x),
        f2 * (deltaUV22.y * edge12.y - deltaUV12.y * edge22.y),
        f2 * (deltaUV22.y * edge12.z - deltaUV12.y * edge22.z) };

    glm::vec3 bitangent2{
        f2 * (-deltaUV22.x * edge12.x + deltaUV12.x * edge22.x),
        f2 * (-deltaUV22.x * edge12.y + deltaUV12.x * edge22.y),
        f2 * (-deltaUV22.x * edge12.z + deltaUV12.x * edge22.z) };
    
    tangent1 = glm::normalize(tangent1);
    tangent2 = glm::normalize(tangent2);
    bitangent1 = glm::normalize(bitangent1);
    bitangent2 = glm::normalize(bitangent2);
    
    std::array<float, 84> quadVertices = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

    return quadVertices;
}

int main() {
    // Initialize Window
    auto window = Init();
    glEnable(GL_DEPTH_TEST);
    auto& camera = InitCamera();

    // Quad
    auto quadVertices = CreateQuad();
    texture::Texture2D tex_quad         { "27_textures/brickwall.jpg", GL_RGB };
    texture::Texture2D tex_quad_normal  { "27_textures/brickwall_normal.jpg", GL_RGB };
    auto obj_quad = helper::CreateBindingObject(quadVertices, 14,
                                                { {0, 3, 0}, {1, 3, 3}, {2, 2, 6}, {3, 3, 8}, {4, 3, 11} });

    // LightBox
    helper::Shader shad_lightbox{ "26_shaders/lightbox.vert", "26_shaders/lightbox.frag" };
    light::PointLight pointlight{ glm::vec3(0, 0, 0), .2f, 1, 1 };
    auto obj_lightbox = helper::CreateBindingObject(complete_box, 8, { {0, 3, 0} });

    // Shaders
    helper::Shader shad_normal  { "27_shaders/quad.vert", "27_shaders/quad.frag" };
    shad_normal.Use(); { // Setting
        shad_normal.SetInt("textureDiffuse1", 0);
        shad_normal.SetInt("textureSpecular1", 1);
        shad_normal.SetInt("textureNormal", 2);
        shad_normal.SetStructPointLight("pointlight", pointlight);
    }

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Update
        glm::mat4 light_model;

        // Rendering
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = camera.GetViewMatrix();

        shad_lightbox.Use(); { // Only use for light box.
            shad_lightbox.SetVecMatirix4f("view", view);
            shad_lightbox.SetVecMatirix4f("projection", camera.GetProjection());

            glm::mat4 model;
            auto movement = 12.0f * sin((float)glfwGetTime() * 1.5f);
            model = glm::translate(model, glm::vec3(movement, 0, -movement));
            model = glm::translate(model, pointlight.GetStandPos());
            pointlight.SetPosition(glm::vec3(model * glm::vec4(pointlight.GetStandPos(), 1)));

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
            shad_normal.SetStructPointLight("pointlight", pointlight);
        }

        glm::mat4 common_model;
        common_model = glm::translate(common_model, glm::vec3(0, 0, -5));
        common_model = glm::scale(common_model, glm::vec3(5.0f));
        
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex_quad.GetId());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, tex_quad_normal.GetId());
            
            glm::mat4 _1;
            _1 = glm::rotate(_1, glm::radians(-135.f), glm::vec3(0, 1, 0));
            _1 *= common_model;
            shad_normal.SetVecMatirix4f("model", _1);

            glBindVertexArray(obj_quad.vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            shad_normal.SetVecMatirix4f("model", common_model);
            glBindVertexArray(obj_quad.vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
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