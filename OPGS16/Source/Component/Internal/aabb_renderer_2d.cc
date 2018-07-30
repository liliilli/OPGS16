#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/aabb_renderer_2d.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file.
///

/// Header file
#include <Component/Internal/aabb_renderer_2d.h>

#include <Element/Builtin/Model/model_2dquadline.h>
#include <Manager/scene_manager.h>
#include <Manager/shader_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>
#include <Manager/Internal/vao_management.h>

namespace opgs16::component::_internal {

CPrivateAabbRenderer2D::CPrivateAabbRenderer2D(element::CObject& bind_object) :
    CPrivateAabbRendererBase(bind_object) {
  using phitos::enums::EFound;
  using builtin::model::BModel2DQuadLine;
  using manager::shader::GetShader;
  using manager::_internal::vao::FindVaoResource;

  auto [ptr, result] = FindVaoResource(BModel2DQuadLine::s_model_name);
  PHITOS_ASSERT(result == EFound::Found,
                "Did not find built-in vao items. opbtQuadLine");

  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(GetShader("opQuad2dLineLoop"));
  m_wrapper.SetAttribute(m_weak_vao_ref);
  m_type = EAabbColliderStyle::D2;
}

CPrivateAabbRenderer2D::~CPrivateAabbRenderer2D() {
  m_weak_vao_ref->DecreaseCount();
}

void CPrivateAabbRenderer2D::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  m_wrapper.SetUniformMat4(builtin::s_uniform_model, PGetModelMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());
  m_wrapper.UseShader();

  const auto& vao_list = m_weak_vao_ref->GetVaoList();
  for (const auto& vao : vao_list) {
    glBindVertexArray(vao.GetVaoId());
    glDrawArrays(GL_LINE_LOOP, 0, 4);
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component::_internal namespace