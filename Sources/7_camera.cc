#include <iostream>
#include <memory>
#include <functional>
#include "stb_image.h"

#include "camera.h"
#include "helper.h"
#include "main.h"

#define SIZE_OF(size, type) (void*)(size * sizeof(type))
#define SIZE_NULL (void*)0

float vertex_data[] = {
    // Local Position    // Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

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
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    }

    {
        auto stride = 5 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, SIZE_NULL);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, SIZE_OF(3, float));
        glEnableVertexAttribArray(1);
    }

    // Generate a texture
    auto texture_1 = std::make_unique<helper::Texture2D>("container.jpg", GL_RGB);
    auto texture_2 = std::make_unique<helper::Texture2D>("awesomeface.png", GL_RGBA);
    texture_1->SetTextureParameterI(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<helper::Shader>("6_shaders/6_normal.vert", 
                                                           "6_shaders/6_normal.frag");

    glm::vec3 cube_position[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),  glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),  glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_1->GetId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_2->GetId());

        shader_program->Use();
        auto id = shader_program->GetId();

        {
            glm::mat4 m_view;
            m_view = camera::GetCamera().GetViewMatrix();

            // 카메라에 대해서 회전을 한 뒤에 변경된 축을 토대로 폏행이동 한다.
            glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, 
                               glm::value_ptr(m_view));
            glUniformMatrix4fv(glGetUniformLocation(id, "proj"), 1, GL_FALSE, 
                               glm::value_ptr(m_proj));

            glUniform1i(glGetUniformLocation(shader_program->GetId(), "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_program->GetId(), "texture2"), 1);
        }
 
        for (auto i = 0u; i < 10; ++i) {
            glm::mat4 m_model;
            m_model = glm::translate(m_model, cube_position[i]);
            m_model = glm::rotate(m_model, 0.f, glm::vec3(.5f, 1.f, 0.f));

            glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, 
                               glm::value_ptr(m_model));
     
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime(glfwGetTime());
        camera::GetCamera().Refresh();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}