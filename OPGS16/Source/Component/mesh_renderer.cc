#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/mesh_renderer.cc
///
/// @brief Mesh renderer.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file.
///

/// Header file
#include <Component/mesh_renderer.h>

#include <Manager/shader_manager.h>
#include <Manager/Internal/vao_management.h>

namespace opgs16::component {

CMeshRenderer::CMeshRenderer(element::CObject& bind_object, CMeshImporter& mesh_importer, const std::string& shader_name) :
    CRendererBase{ bind_object }, m_mesh_importer{ &mesh_importer } {
  SetShader(shader_name);
  m_wrapper.SetAttribute(m_mesh_importer->pGetVaoRawRef());
}

CMeshRenderer::CMeshRenderer(element::CObject& bind_object, CMeshImporter& mesh_importer) :
  CMeshRenderer{ bind_object, mesh_importer, "opMesh3dDefault" } {
}

element::CShaderWrapper& CMeshRenderer::Getwrapper() noexcept {
  return m_wrapper;
}

void CMeshRenderer::SetShader(const std::string& shader_name) {
  using opgs16::manager::shader::GetShader;
  m_wrapper.SetShader(GetShader(shader_name));
}

void CMeshRenderer::Render() {
  m_wrapper.UseShader();

  auto& list = m_mesh_importer->pGetVaoRawRef()->GetVaoList();
  for (const auto& vao : list) {
    glBindVertexArray(vao.GetVaoId());
#ifdef false
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sprite->GetTextureId());
#endif

    const bool has_indice = (vao.GetEboId() != 0);
    int32_t count = 0;
    if (has_indice) {
      count = vao.GetIndiceCount();
    }
    else {
      count = vao.GetVertexCount();
    }

    if (has_indice)
      glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, 1);
    else
      glDrawArraysInstanced(GL_TRIANGLES, 0, count, 1);
  }

#ifdef false
  glBindTexture(GL_TEXTURE_2D, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component namespace