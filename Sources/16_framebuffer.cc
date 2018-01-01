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

template <typename _Ty>
constexpr void* GetOffset(const int i) {
    return (void*)(i * sizeof(_Ty));
}

int main() {
    // Initialize Window
    auto window = Init();
    stbi_set_flip_vertically_on_load(true);

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VAO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO; {
        // Bind vertex data to be used into Vertex Buffer Object 
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(box_data), box_data, GL_STATIC_DRAW);
        
        const float stride = 5 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, GetOffset<float>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, GetOffset<float>(3));
        glEnableVertexAttribArray(2);
    }
    glBindVertexArray(0);

    // Set up VAO, VBO, EBO for quad drawn on default frame buffer.
    GLuint quad_vao;
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
    GLuint quad_vbo; {
        glGenBuffers(1, &quad_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_info), quad_info, GL_STATIC_DRAW);

        const float stride = 8 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, GetOffset<float>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, GetOffset<float>(6));
        glEnableVertexAttribArray(1);
    }
    GLuint quad_ebo; {
        glGenBuffers(1, &quad_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);

    // Make Texture container.
    helper::Texture2D texture_floor{ "15_textures/checker_2.png", GL_RGBA };
    texture_floor.SetTextureParameterI({ 
        std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE),
        std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE),
        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_NEAREST),
        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_NEAREST) });
    helper::Texture2D texture_smile{ "15_textures/smile.png", GL_RGBA };
   

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


    // Set user-custom framebuffer
    GLuint frame_buffer;
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

    // Make texture for specific user-defined framebuffer
    GLuint texture_color_buffer;
    glGenTextures(1, &texture_color_buffer);
    glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to currently bound framebuffer object.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture_color_buffer, 0);

    // Make renderbuffer for depth & stencil testing
    GLuint render_buffer;
    glGenRenderbuffers(1, &render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, render_buffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Make shader program with vs and fs path.
    helper::Shader shader_box{"14_shaders/normal.vert", "14_shaders/window.frag"};
    helper::Shader shader_object{"16_shaders/normal.vert", "14_shaders/window.frag"};
    helper::Shader shader_rotation{ "16_shaders/rotation.vert", "14_shaders/window.frag" };
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/gaussian.frag"};
    helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/retrieve.frag"};
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/edge.frag"};
    //helper::Shader shader_screen{"16_shaders/retrieve.vert" , "16_shaders/grayscale.frag"};

    // ************************************************************************
    // Set Camera
    camera::SetCursor(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
    auto& camera = camera::GetCamera();
    camera.SetPerspective(50.f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.f);
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

        shader_object.Use(); {
            shader_object.SetInt("texture_diffuse1", 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_floor.GetId());

            glBindVertexArray(quad_vao);
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

            glBindVertexArray(quad_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        // First pass
        // Draw objects on user-defined frame buffer and write Color values on buffer.
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(.05f, .05f, .05f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_box.Use(); {
            m_view = camera.GetViewMatrix();
            shader_box.SetVecMatirix4f("view", m_view);
            shader_box.SetVecMatirix4f("projection", camera.GetProjection());
            shader_box.SetInt("texture_diffuse1", 0);

            {
                glm::mat4 model;
                shader_box.SetVecMatirix4f("model", model);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, box_texture_cb);

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }

        // Second pass
        // Draw result color buffer onto Default frame buffer.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_screen.Use(); {
            shader_screen.SetInt("screenTexture", 0);

            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_color_buffer);

                glBindVertexArray(quad_vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
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