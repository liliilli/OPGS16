#include "light_box.h"

LightBox::LightBox() :
    pointlight{ position, glm::vec3(1) * .2f, glm::vec3(1), glm::vec3(1) } {}

void LightBox::Draw(helper::ShaderNew & shader) {
    /** Refresh Model matrix */
    GetModelMatrix();
    shader.SetVecMatrix4f("uModel", model);

    glBindVertexArray(object_info.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void LightBox::SetUpLight(const int index, helper::ShaderNew& shader) {
    shader.SetStructPointLight("uPointLight[" + std::to_string(index) + "]",
                               pointlight);
}

void LightBox::SetPosition(glm::vec3 position) {
    Object::SetPosition(position);
    pointlight.SetPosition(this->position);
}
