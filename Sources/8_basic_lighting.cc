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
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), 
                      static_cast<float>(SCREEN_HEIGHT / 2));
    glEnable(GL_DEPTH_TEST);

    // Image load upright, not to be displayed as inverted texture.
    stbi_set_flip_vertically_on_load(true);


    glm::mat4 m_proj;
    m_proj = glm::perspective(glm::radians(50.f),
                              static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT,
                              0.1f, 100.f);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_data), box_data, GL_STATIC_DRAW);

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

    GLuint lightVAO;
    {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, SIZE_NULL);
        glEnableVertexAttribArray(0);
    }

    // Make shader program with vs and fs path.
    auto shader_object = std::make_unique<helper::Shader>("8_shaders/8_diffuse.vert", 
                                                          "8_shaders/8_specular.frag");
    auto shader_source = std::make_unique<helper::Shader>("8_shaders/8_normal.vert",
                                                          "8_shaders/8_source.frag");

    auto light_standard = glm::vec3(1.0f, 1.0f, -1.0f);
    auto intensity = 3.0f;
    glm::vec3 light_position;

    glm::vec3 cube_position[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),  glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),  glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Loop
    auto& camera = camera::GetCamera();

    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        auto time = static_cast<float>(glfwGetTime()) * 2;
        light_position = light_standard + glm::vec3(intensity * sinf(time),
                                                    intensity * cosf(time),
                                                    0);
        
        // Rendering
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 m_view;
        m_view = camera.GetViewMatrix();

        // Object
        shader_object->Use(); 
        for (auto i = 0u; i < 10u; ++i) {
            GLuint id = shader_object->GetId();

            shader_object->SetVecMatirix4f("view", m_view);
            shader_object->SetVecMatirix4f("projection", m_proj);

            glm::mat4 m_model;
            m_model = glm::translate(m_model, cube_position[i]);
            glm::mat4 inverse_model = glm::inverse(m_model);

            shader_object->SetVecMatirix4f("modelNormal", inverse_model);
            shader_object->SetVecMatirix4f("model", m_model);
            shader_object->SetVec3f("objectColor", 1.f, .5f, .31f);
            shader_object->SetVec3f("lightColor", 1.f, 1.f, 1.f);
            shader_object->SetVec3f("lightPos", 
                                    light_position.x, 
                                    light_position.y, 
                                    light_position.z);
            shader_object->SetVec3f("viewPos", camera.GetPosition());
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
            shader_source->SetVecMatirix4f("projection", m_proj);
   
            shader_source->SetVecMatirix4f("model", m_model);
            shader_source->SetVec3f("lightColor", 1.f, 1.f, 1.f);
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