#include "bezier_quad.h"

#include <glm/gtc/matrix_transform.hpp>
#include "..\GlobalObjects\camera.h"
#include "..\System\Frame\constant.h"

BezierQuad::BezierQuad() {
    quad = helper::CreateBindingObjectEBO(quad_info, 8,
                                          { { 0, 3, 0 },{ 1, 3, 3 },{ 2, 2, 6 } },
                                          quad_indices);

    glGenVertexArrays(1, &bezier_vao);
    glBindVertexArray(bezier_vao);

    glGenBuffers(1, &bezier_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, bezier_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_bezier_patches), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &bezier_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezier_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(m_bezier_indices), &m_bezier_indices[0], GL_STATIC_DRAW);
}

void BezierQuad::Update() {
    glBindVertexArray(bezier_vao);
    glBindBuffer(GL_ARRAY_BUFFER, bezier_vbo);
    auto* ptr = (glm::vec3*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(m_bezier_patches),
                                             GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(ptr, &m_bezier_patches[0], sizeof(m_bezier_patches));

    for (auto i = 0; i < 16; ++i) {
        float fi = static_cast<float>(i) / 16.0f;
        ptr[i][2] = std::sinf(4.f * (float)glfwGetTime() * (0.2f + fi * 0.3f));
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(0);
}

void BezierQuad::Draw(helper::ShaderNew& shader) {
    glPatchParameteri(GL_PATCH_VERTICES, 16);

    glBindVertexArray(bezier_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_check.GetId());

    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(2.f));

    auto mv = camera::GetCamera().GetViewMatrix() * model;
    shader.SetVecMatrix4f("mv", mv);
    shader.SetVecMatrix4f("projection", camera::GetCamera().GetProjection());

    glDrawArrays(GL_PATCHES, 0, 16);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
}
