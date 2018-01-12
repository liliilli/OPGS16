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
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    // Set Camera
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);

    // Make shader program with vs and fs path.
    helper::Shader shader_object{"11_shaders/normal.vert", "11_shaders/normal.frag"};
    helper::Shader shader_source{"8_shaders/8_normal.vert", "8_shaders/8_source.frag"};

    // Generate models and textures
    model::Model car{"bmw/BMW X5 4.obj"};
    model::Model man{"nanosuit/nanosuit.obj"};

    // Floor
    const float floor_info[] = {
        // Vertex       //Normal        // TexCoord
        1.f, 1.f, 0.f,  0.f, 1.f, 0.f,  1.f, 1.f,
        1.f,-1.f, 0.f,  0.f, 1.f, 0.f,  1.f, 0.f,
       -1.f,-1.f, 0.f,  0.f, 1.f, 0.f,  0.f, 0.f,
       -1.f, 1.f, 0.f,  0.f, 1.f, 0.f,  0.f, 1.f
    };
    const unsigned floor_face[] = {
        // Primitive is triangle
        0, 1, 2,
        2, 3, 0
    };

    GLuint floor_vao, floor_vbo, floor_ebo;
    glGenVertexArrays(1, &floor_vao);
    glGenBuffers(1, &floor_vbo);
    glGenBuffers(1, &floor_ebo);

    glBindVertexArray(floor_vao); {
        glBindBuffer(GL_ARRAY_BUFFER, floor_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floor_info), floor_info, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_face), floor_face, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    stbi_set_flip_vertically_on_load(true);
    GLuint texture; {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set the texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image file to use as texture image.
        auto width = 0;
        auto height = 0;
        auto nr_channels = 0;
        auto data = stbi_load("11_textures/grass.jpg", &width, &height, &nr_channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else { std::cerr << "FAILED::LOAD::TEXTURE" << std::endl; }
        stbi_image_free(data);
    }

    // Light box

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    // Bind vertex data to be used into Vertex Buffer Object 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(map_vertices), map_vertices, GL_STATIC_DRAW);

    // Generate and bind VAO is used for emitting light;
    GLuint lightVAO; {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Make light structures
    helper::DirectionalLight dir_light{
        glm::vec3(0, -1, 0),    // Direciton
        glm::vec3(1) * 0.2f,    // Ambient
        glm::vec3(1),           // Diffuse
        glm::vec3(1)            // Specular
    };

    // variables related to light value and motions
    const glm::vec3 light_color_init{ 1.f, 1.f, 1.f };
    const glm::vec3 light_standard{ 0, 0, 0 };
    const glm::vec3 light_front_std{ -1, -1, 0 };

    glm::vec3 light_position;
    glm::vec3 light_front;

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        ProcessInput(window);

        auto time           = static_cast<float>(glfwGetTime());
        auto light_color    = light_color_init;

        glm::mat4 l_rotate;
        l_rotate    = glm::rotate(l_rotate, time, glm::vec3(0, 0, 1));

        glm::mat4 l_translate;
        l_translate = glm::translate(l_translate,
                                     4.0f * glm::vec3(l_rotate * glm::vec4(1, 1, 0, 0)));

        light_position  = glm::vec3(l_translate * glm::vec4(light_standard, 1.0));
        light_front     = glm::vec3(l_rotate * glm::vec4(light_front_std, 0.0));

        // Rendering
        glClearColor(.05f, .05f, .05f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use model shader object
        shader_object.Use();

        glm::mat4 m_view = camera.GetViewMatrix();
        shader_object.SetVecMatirix4f("view"    , m_view);
        shader_object.SetVecMatirix4f("projection", camera.GetProjection());
        
        // light setting
        shader_object.SetStructDirLight("uDirLight", dir_light);

        helper::Spotlight spotlight{
            glm::vec3(m_view * glm::vec4(light_front, 0)),
            glm::vec3(m_view * glm::vec4(light_position, 1)),
            0.2f * glm::vec3(1), glm::vec3(1), glm::vec3(1),
            1.0f, 0.045f, 0.0075f,
            glm::cos(glm::radians(35.f)), glm::cos(glm::radians(40.f)) };
        shader_object.SetStructSpotlight("uSpotlight", spotlight);

        // set car position and draw
        {
            glm::mat4 m_model;
            m_model = glm::translate(m_model, glm::vec3(0, 0, 1));
            m_model = glm::scale(m_model, glm::vec3(0.04f));
            shader_object.SetVecMatirix4f("model", m_model);
            car.Draw(shader_object);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        // set man position and draw
        {
            glm::mat4 m_model;
            m_model = glm::translate(m_model, glm::vec3(0, 0, -2.f));
            m_model = glm::rotate(m_model, glm::radians(-90.f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(0.2f));
            shader_object.SetVecMatirix4f("model", m_model);
            man.Draw(shader_object);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        // floor rendering
        GLuint id = shader_object.GetId();
        {
            glm::mat4 m_model;
            m_model = glm::rotate(m_model, glm::radians(-90.f), glm::vec3(1, 0, 0));
            m_model = glm::scale(m_model, glm::vec3(4.f));

            shader_object.SetVecMatirix4f("model", m_model);
            shader_object.SetInt("texture_diffuse1", 0);
            shader_object.SetInt("texture_specular1", 1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture);

            glBindVertexArray(floor_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // Light source rendering
        shader_source.Use();
        {
            GLuint id = shader_source.GetId();

            glm::mat4 m_model;
            m_model = glm::translate(m_model, light_position);
            m_model = glm::scale(m_model, glm::vec3(0.5f));

            shader_source.SetVecMatirix4f("model", m_model);
            shader_source.SetVecMatirix4f("view", m_view);
            shader_source.SetVecMatirix4f("projection", camera.GetProjection());
            shader_source.SetVec3f("lightColor", glm::vec3(1));

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
