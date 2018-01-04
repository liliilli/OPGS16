
    // Make shader program with vs and fs path.
    auto shader_object = std::make_unique<helper::Shader>("8_shaders/8_phong.vert", 
                                                          "8_shaders/8_phong.frag");
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
        light_position = light_standard + glm::vec3(intensity * sinf(time * 2),
                                                    intensity * cosf(time * 2),
                                                    0);
        glm::vec3 light_color = 
            light_color_init - glm::vec3(abs(sin(time)), 
                                         abs(sin(time - light_offset)), 
                                         abs(sin(time - 2 * light_offset)));
        
        // Rendering
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 m_view = camera.GetViewMatrix();

        // Object
        shader_object->Use(); 
        for (auto i = 0u; i < 10u; ++i) {
            GLuint id = shader_object->GetId();

            shader_object->SetVecMatirix4f("view", m_view);
            shader_object->SetVecMatirix4f("projection", camera.GetProjection());

            glm::mat4 m_model;
            m_model = glm::translate(m_model, cube_position[i]);

            shader_object->SetVecMatirix4f("model", m_model);
            shader_object->SetVec3f("lightPos", light_position);
            shader_object->SetVec3f("lightColor", light_color);
            shader_object->SetVec3f("objectColor", 1.f, .5f, .31f);
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
            shader_source->SetVec3f("lightColor", light_color);
            glBindVertexArray(lightVAO);
     
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        RefreshDeltaTime(glfwGetTime());
        camera::GetCamera().Refresh();
    }