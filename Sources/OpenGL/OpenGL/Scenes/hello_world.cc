#include "hello_world.h"

#include <random>
#include "..\System\Frame\helper.h"
#include "..\System\Frame\constant.h"

HelloWorld::HelloWorld() :
    font{ "Resources/LSANS.TTF" },
    shad_quad{ helper::Shader{ "Shaders/quad.vert", "Shaders/quad.frag" } } {
    std::cout << "Scene Created" << std::endl;

    quad = helper::CreateBindingObjectEBO(quad_info, 8,
                                          { { 0, 3, 0 },{ 1, 3, 3 },{ 2, 2, 6 } },
                                          quad_indices);

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ubo_data), &ubo_data[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Make data for buffer_matrix_object
    std::random_device rd;
    std::mt19937 seed{ rd() };

    std::normal_distribution<> device{ 0, 6 };
    for (auto& item : buffer_object) {
        glm::mat4 model;
        GLfloat x = static_cast<float>(device(seed));
        GLfloat z = static_cast<float>(device(seed));
        model = glm::translate(model, glm::vec3(x, 0, z));

        item = model;
    }

    glBindVertexArray(quad.vao);

    GLuint model_buffer;
    glGenBuffers(1, &model_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_object), &buffer_object[0], GL_STATIC_DRAW);

    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 16 * 4, (void*)0);
    glEnableVertexAttribArray(10);
    glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 4 * 16, (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(11);
    glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, 4 * 16, (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(12);
    glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, 4 * 16, (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(13);

    glVertexAttribDivisor(10, 1);
    glVertexAttribDivisor(11, 1);
    glVertexAttribDivisor(12, 1);
    glVertexAttribDivisor(13, 1);

    // Indirect Instancing
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, quad_count * sizeof(elc_cont), nullptr, GL_DYNAMIC_DRAW);

    auto* cmd = (GLuint*)
        glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, quad_count * sizeof(elc_cont),
                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (size_t i = 0; i < quad_count; ++i) {
        elc_cont[4] = i; // gl_InstanceID
        memcpy(cmd, elc_cont, sizeof(elc_cont));

        elc_cont[3] += 4;
        cmd += sizeof(elc_cont);
    }
    glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
    glBindVertexArray(0);
}

void HelloWorld::HandleInput(GLFWwindow* const window) {
    Scene::HandleInput(window);
}

void HelloWorld::Update() {
    ubo_data[0] = camera.GetViewMatrix();
    ubo_data[1] = camera.GetProjection();

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    auto* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &ubo_data[0], sizeof(ubo_data));
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    camera.Refresh();
}

void HelloWorld::Draw() {

    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    shad_quad.Use();
    auto i = glGetUniformBlockIndex(shad_quad.GetId(), "Matrix");
    glBindBufferBase(GL_UNIFORM_BUFFER, i, ubo);

    glBindVertexArray(quad.vao);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, quad_count, 0);

    font.RenderText("Hello world!", { 25.f, 25.f }, 1.0f, { 1.f, 1.f, 1.f });
}

