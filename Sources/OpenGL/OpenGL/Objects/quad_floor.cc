#include "quad_floor.h"
#include "..\constant.h"

QuadFloor::QuadFloor() :
    object_info{ helper::CreateBindingObjectEBO(quad_info, 8,
                                                {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices) } 
{ }

void QuadFloor::Draw(helper::ShaderNew & shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetId());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
