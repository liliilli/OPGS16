
int main() {
    // Initialize Window
    auto window = init();

    // Data Setting
    float vertex_data[] = {
        -1.f,   -1.f,   +.0f,
        +1.f,   -1.f,   +.0f,
        +.0f,   +1.f,   +.0f,
    };

    // Set up Vertex Buffer Object, Vetex Array Object, and etc.
    GLuint VBO, VAO;
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

        auto stride = 3 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Make Shader program with vs and fs path.
    auto shader_program = std::make_unique<Shader>("3_plain.vert", "3_plain.frag");
    
    // Loop
    while (!glfwWindowShouldClose(window)) {
        // Input 
        process_input(window);

        // Rendering
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            shader_program->Use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
