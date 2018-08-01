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

CPrivateAabbRenderer2D::CPrivateAabbRenderer2D(
    element::CObject& bind_object,
    component::CColliderBox2D* parent) :
    CPrivateAabbRendererBase{bind_object, parent} {
  using phitos::enums::EFound;
  using builtin::model::BModel2DQuadLine;
  using manager::shader::GetShader;
  using manager::_internal::vao::FindVaoResource;

  m_type = EAabbColliderDmStyle::D2;
  auto [ptr, result] = FindVaoResource(BModel2DQuadLine::s_model_name);
  PHITOS_ASSERT(result == EFound::Found,
                "Did not find built-in vao items. opbtQuadLine");

  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(GetShader("opQuad2dLineLoop"));
  m_wrapper.SetAttribute(m_weak_vao_ref);
  m_wrapper.SetUniformFloat(builtin::s_uniform_alpha, 0.5f);
}

CPrivateAabbRenderer2D::~CPrivateAabbRenderer2D() {
  m_weak_vao_ref->DecreaseCount();
}

void CPrivateAabbRenderer2D::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  m_wrapper.SetUniformMat4(builtin::s_uniform_model, pGetModelMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());

  pSetAabbRenderingColor();
  m_wrapper.SetUniformVec3("uColor", m_state_color);

  m_wrapper.UseShader();

  const auto& vao_list = m_weak_vao_ref->GetVaoList();
  glBindVertexArray(vao_list[0].GetVaoId());
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component::_internal namespace