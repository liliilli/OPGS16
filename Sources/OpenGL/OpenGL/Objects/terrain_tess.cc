#include "terrain_tess.h"
#include "..\camera.h"

TerrainTess::TerrainTess() : 
    m_tex_height{ texture::Texture2D{ "Resources/heightmap.jpg", GL_RGB } },
    m_tex_color{ texture::Texture2D{ "Resources/color.jpg", GL_RGB } } {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void TerrainTess::Update() {
    glm::mat4 model;

    auto& camera = camera::GetCamera();
    mvp = camera.GetProjection() * camera.GetViewMatrix() * model;
}

void TerrainTess::Draw(helper::ShaderNew & shader) {
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    shader.SetVecMatrix4f("mvp", mvp);
    shader.SetFloat("dmap_depth", 10.0f);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex_height.GetId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_tex_color.GetId());
    glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);

    glPatchParameteri(GL_PATCH_VERTICES, 3);

}
