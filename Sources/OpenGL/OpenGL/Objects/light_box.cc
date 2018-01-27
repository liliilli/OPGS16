#include "light_box.h"
#include "..\System\Frame\constant.h"
#include "..\System\Frame\helper.h"

LightBox::LightBox() :
    pointlight{ GetLocalPosition(), glm::vec3(1) * .2f, glm::vec3(1), glm::vec3(1) },
    object_info{ helper::CreateBindingObject(complete_box, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}) }
{}

void LightBox::Draw(helper::ShaderNew & shader) {
    /** Refresh Model matrix */
    GetModelMatrix();
    shader.SetVecMatrix4f("uModel", GetModelMatrix());
    shader.SetVec3f("uColor", pointlight.GetDiffuse());

    glBindVertexArray(object_info.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void LightBox::SetUpLight(const int index, helper::ShaderNew& shader) {
    shader.SetStructPointLight("uPointLight[" + std::to_string(index) + "]", pointlight);
}

void LightBox::SetLocalPosition(glm::vec3 position) {
    Object::SetLocalPosition(position);
    pointlight.SetLocalPosition(GetLocalPosition());
}

void LightBox::SetColor(glm::vec3 color) {
    pointlight.SetColor(color);
}
