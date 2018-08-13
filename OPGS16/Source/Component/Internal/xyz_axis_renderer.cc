#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/xyz_axis_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file.
///

/// Header file
#include <Component/Internal/xyz_axis_renderer.h>
#include <glm/gtx/rotate_vector.hpp>

#include <Element/object.h>
#include <Element/Builtin/Model/model_empty.h>
#include <Manager/scene_manager.h>
#include <Manager/shader_manager.h>
#include <Manager/Internal/vao_management.h>
#include <Manager/Internal/shader_builtin_keywords.h>

namespace opgs16::component::_internal {

CPrivateXyzAxisRenderer::CPrivateXyzAxisRenderer(element::CObject& bind_object) :
    CRendererBase{ bind_object } {
  using phitos::enums::EFound;
  using manager::shader::GetShader;
  using manager::_internal::vao::FindVaoResource;

  auto [ptr, result] = FindVaoResource(builtin::model::BModelEmpty::s_model_name);
  PHITOS_ASSERT(result == EFound::Found, "Did not find built-in vao items. opbtEmpty");

  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(GetShader("opXyzAxis"));
  m_wrapper.SetAttribute(m_weak_vao_ref);
}

void CPrivateXyzAxisRenderer::Update(float delta_time) {
  auto& obj = GetBindObject();

  const auto& axis_list = obj.pfGetObjectWorldSpaceAxis();
  m_x_normalized = axis_list[0];
  m_y_normalized = axis_list[1];
  m_z_normalized = axis_list[2];

  m_wrapper.SetUniformVec3("uPosition", obj.GetFinalPosition());

  manager::object::pBindRenderXyzAxisRenderer(*this);
}

void CPrivateXyzAxisRenderer::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  m_wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());
  m_wrapper.SetUniformVec3("uX", m_x_normalized * 8);
  m_wrapper.SetUniformVec3("uY", m_y_normalized * 8);
  m_wrapper.SetUniformVec3("uZ", m_z_normalized * 8);
  m_wrapper.UseShader();

  const auto& vao_list = m_weak_vao_ref->GetVaoList();
  glBindVertexArray(vao_list[0].GetVaoId());
  glDrawArrays(GL_LINES, 0, 6);

  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component::_internal namespace