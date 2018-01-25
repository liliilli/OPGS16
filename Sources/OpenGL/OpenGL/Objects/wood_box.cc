#include "wood_box.h"
#include "..\System\Frame\constant.h"

WoodBox::WoodBox() :
    object_info{ helper::CreateBindingObject(complete_box, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}) }
{}

void WoodBox::Draw(helper::ShaderNew& shader) {
    /** Refresh Model matrix */
    GetModelMatrix();
    shader.SetVecMatrix4f("uModel", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_specular.GetId());

    glBindVertexArray(object_info.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
}
