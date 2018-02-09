#include "transform_feedback.h"

#include <random>
#include "..\System\Frame\helper.h"
#include "..\System\Frame\constant.h"

TfFeedback::TfFeedback() :
    font{ "Resources/LSANS.TTF" },
    m_shad_update { helper::Shader{ "Shaders/spring.vert" } },
    m_shad_render{ helper::Shader{ "Shaders/spring_render.vert", "Shaders/spring_render.frag" } },
    shad_quad{ helper::Shader{ "Shaders/quad.vert", "Shaders/quad.frag" } } {

    std::array<const char*, 2> varyings = { "tfPositionMass", "tfVelocity" };
    glTransformFeedbackVaryings(m_shad_update.GetId(), 2, &varyings[0], GL_SEPARATE_ATTRIBS);
    glLinkProgram(m_shad_update.GetId());

    //SetInitialNodes();

    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    //GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    //GLuint vbo;
    //glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

void TfFeedback::SetInitialNodes() {
    auto n = 0;
    std::array<glm::vec4, k_x * k_y> m_initial_positions{};
    std::array<glm::vec3, k_x * k_y> m_initial_velocity{};
    std::array<glm::ivec4, k_x * k_y> m_connection_index{};

    for (int j = 0; j < k_y; ++j) {
        float fj = (float)j / (float)k_y;

        for (int x = 0; x < k_x; ++x) {
            float fx = (float)x / (float)k_x;

            auto& pos = m_initial_positions[n];
            pos = glm::vec4{ (fx - .5f) * k_x, (fj - .5f) * k_y, .6f * sinf(fx) * cosf(fj), 1.f };
            m_initial_velocity[n] = glm::vec3{ 0.f };
            m_connection_index[n] = glm::ivec4{ -1 };

            if (j != k_y - 1) {
                if (x != 0) // Left
                    m_connection_index[n][0] = n - 1;

                if (j != 0) // Up
                    m_connection_index[n][1] = n - k_x;

                if (x != k_x - 1) // Right
                    m_connection_index[n][2] = n + 1;

                if (j != k_y - 1)
                    m_connection_index[n][3] = n + k_x;
            }

            ++n;
        }
    }

    // Bind buffers
    glGenVertexArrays(2, &m_vao[0]);
    glGenVertexArrays(5, &m_vbo[0]);

    for (auto i = 0; i < 2; ++i) {
        glBindVertexArray(m_vao[i]);

        // Bind Position data
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_initial_positions),
                     &m_initial_positions[0], GL_DYNAMIC_COPY);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Bind Velocity data
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2 + i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_initial_velocity),
                     &m_initial_velocity[0], GL_DYNAMIC_COPY);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        // Bind Neighbor node data
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_connection_index),
                     &m_connection_index[0], GL_STATIC_DRAW);
        glVertexAttribIPointer(2, 4, GL_INT, 0, 0);
        glEnableVertexAttribArray(2);
    }

    // Attach a pair of buffer to Texture buffer object;
    glGenTextures(2, &m_pos_tbo[0]);
    for (auto i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[i]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vbo[i]);
    }
}

void TfFeedback::HandleInput(GLFWwindow* const window) {
    Scene::HandleInput(window);
}

void TfFeedback::Update() {
    //m_shad_update.Use();
    //glEnable(GL_RASTERIZER_DISCARD);

    //glBindVertexArray(m_vao[buffer_index & 1]);
    //glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[buffer_index & 1]);

    //GLfloat feedback[5];
    //glGetBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(feedback), feedback);
    //for (int i = 0; i < 5; ++i) {
    //    std::cerr << feedback[i];
    //}   std::cout << std::endl;

    //buffer_index ^= 1;
    //glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[buffer_index & 1]);
    //glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vbo[2 + (buffer_index & 1)]);
    //
    //glBeginTransformFeedback(GL_POINTS);
    //glDrawArrays(GL_POINTS, 0, (int)(k_x * k_y));
    //glEndTransformFeedback();

    //glDisable(GL_RASTERIZER_DISCARD);
    //glFlush();

    //camera.Refresh();
}

void TfFeedback::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shad_render.Use();
    glPointSize(4.0f);
    //glBindVertexArray(m_vao[buffer_index & 1]);
    //glDrawArrays(GL_POINTS, 0, k_x * k_y);

    font.RenderText("TfFeedback Scene", { 25.f, 25.f }, 1.0f, { 1.f, 1.f, 1.f });
}
