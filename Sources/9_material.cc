#include <iostream>
#include <memory>
#include <functional>
#include "stb_image.h"

#include "camera.h"
#include "constant.h"
#include "helper.h"
#include "main.h"

#define SIZE_OF(size, type) (void*)(size * sizeof(type))
#define SIZE_NULL (void*)0

int main() {
    // Initialize Window
    auto window = Init();
    glEnable(GL_DEPTH_TEST);

    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), 
                      static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 
                          0.1f, 100.f);

    // Image load upright, not to be displayed as inverted texture.
    stbi_set_flip_vertically_on_load(true);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    // Bind vertex data to be used into Vertex Buffer Object 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_data), box_data, GL_STATIC_DRAW);

    // Generate and Bind vertex array object which is used for be-lightened object.
    auto stride = 6 * sizeof(float);
    GLuint VAO;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, SIZE_NULL);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, SIZE_OF(3, float));
        glEnableVertexAttribArray(1);
    }

    // Generate and bind VAO is used for emitting light;
    GLuint lightVAO;
    {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, SIZE_NULL);
        glEnableVertexAttribArray(0);
    }

    // Make shader program with vs and fs path.
    auto shader_object = std::make_unique<helper::Shader>("9_shaders/9_phong.vert", 
                                                          "9_shaders/9_phong.frag");
    auto shader_source = std::make_unique<helper::Shader>("8_shaders/8_normal.vert",
                                                          "8_shaders/8_source.frag");

    glm::vec3 cube_position[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),  glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),  glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // variables related to light
    const auto light_offset = glm::pi<float>() * 2.0f / 3.0f;
    const auto intensity = 3.0f;
    auto light_standard = glm::vec3(1.0f, 1.0f, -1.0f);
    glm::vec3 light_position;
    const auto light_color_init = glm::vec3(1.f, 1.f, 1.f);

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        auto time = static_cast<float>(glfwGetTime());
        // View space coordinate
        light_position = light_standard;
        auto light_color = light_color_init;
        
        // Rendering
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 m_view = camera.GetViewMatrix();

        // Object
        shader_object->Use(); 
        for (auto i = 0u; i < 10u; ++i) {
            GLuint id = shader_object->GetId();

            glm::mat4 m_model;
            m_model = glm::translate(m_model, cube_position[i]);

            // Vertex Shader uniform variables
            shader_object->SetVecMatirix4f("model", m_model);
            shader_object->SetVecMatirix4f("view", m_view);
            shader_object->SetVecMatirix4f("projection", camera.GetProjection());

            shader_object->SetVec3f("lightPosition", light_position);

            // Fragment Shader uniform variables
            shader_object->SetVec3f("Light.ambient", 0.2f * light_color);
            shader_object->SetVec3f("Light.diffuse", glm::vec3(0, 0, 1));
            shader_object->SetVec3f("Light.specular", light_color);

            shader_object->SetVec3f("uMaterial.ambient", 1.f, .5f, .31f);
            shader_object->SetVec3f("uMaterial.diffuse", 1.f, .5f, .31f);
            shader_object->SetVec3f("uMaterial.specular", 1.f, .5f, .31f);
            shader_object->SetFloat("uMaterial.intensity", 64.0f);
            glBindVertexArray(VAO);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Lamp
        shader_source->Use(); 
        {
            GLuint id = shader_source->GetId(); 
            glm::mat4 m_model;
            m_model = glm::translate(m_model, light_position);
            m_model = glm::scale(m_model, glm::vec3(0.5f));

            shader_source->SetVecMatirix4f("view", m_view);
            shader_source->SetVecMatirix4f("projection", camera.GetProjection());
   
            shader_source->SetVecMatirix4f("model", m_model);
            shader_source->SetVec3f("lightColor", glm::vec3(0, 0, 1));
            glBindVertexArray(lightVAO);
     
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime(glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}