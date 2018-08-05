#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_renderer.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-05 Create file.
///

/// Header file
#include <Component/Internal/particle_renderer.h>

#include <Element/object.h>
#include <Element/Builtin/Model/model_point.h>

#include <Manager/texture_manager.h>
#include <Manager/shader_manager.h>
#include <Headers/import_logger.h>

#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>
#include <Manager/Internal/vao_management.h>

namespace {

constexpr const char* s_particle_shader_name = "opParticle";

constexpr const char* s_shader_uColor = "uColor";
constexpr const char* s_shader_inPosition = "inPosition";
constexpr const char* s_shader_ptcSize = "ptcSize";

} /// unnamed namespace

namespace opgs16::component::_internal {

CInternalParticleRenderer::CInternalParticleRenderer(element::CObject& bind_object) :
    CRendererBase { bind_object } {
  using manager::_internal::vao::FindVaoResource;
  using manager::shader::GetShader;
  using manager::texture::GetTexture;
  using builtin::model::BModelPoint;
  using phitos::enums::EFound;

  auto [ptr, result] = FindVaoResource(BModelPoint::s_model_name);
  PHITOS_ASSERT(result == EFound::Found, "Did not find built-in vao items. opQuad2d");
  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(GetShader(s_particle_shader_name));
  m_wrapper.SetAttribute(m_weak_vao_ref);
  SetTexture("opSystem");
}

void CInternalParticleRenderer::SetTexture(const std::string& texture_name) {
  const auto texture = manager::texture::GetTexture(texture_name);
  if (!texture) {
    PUSH_LOG_ERROR_EXT("Failed to find texture {}.", texture_name);
    m_sprite = manager::texture::GetTexture("opSystem");
  }
  else {
    m_sprite = texture;
  }
  SetTextureFragmentIndex(0);
}

void CInternalParticleRenderer::SetTextureFragmentIndex(int32_t index_value) {
  using ETexelType = texture::CTexture2D::ETexelType;
  PHITOS_ASSERT(index_value >= 0, "Texture fragment index must be bigger than 0 or equal.");

  if (!m_sprite->DoesHasAtlas()) {
    PUSH_LOG_WARN("Bound texture does not have atlas information. Failed to assign new_index.");
    pResetTextureFragmentProperties();
    return;
  }

  m_texture_fragment_index = index_value;
  const auto texel_ptr_ld = m_sprite->GetTexelPtr(ETexelType::LEFT_DOWN, index_value);
  const auto texel_ptr_ru = m_sprite->GetTexelPtr(ETexelType::RIGHT_UP, index_value);

  if (texel_ptr_ld && texel_ptr_ru) {
    m_wrapper.SetUniformVec2(builtin::s_uniform_texelld, texel_ptr_ld.value());
    m_wrapper.SetUniformVec2(builtin::s_uniform_texelru, texel_ptr_ru.value());
  }
  else {
    PUSH_LOG_WARN("Any getting texel from resource has been failed. Texel is assigned to overall region.");
    pResetTextureFragmentProperties();
  }
}

void CInternalParticleRenderer::pResetTextureFragmentProperties() {
  m_texture_fragment_index = 0;
  m_wrapper.SetUniformVec2(builtin::s_uniform_texelld, { 0.f, 0.f });
  m_wrapper.SetUniformVec2(builtin::s_uniform_texelru, { 1.f, 1.f });
}

void CInternalParticleRenderer::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  m_wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  m_wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());
  m_wrapper.SetUniformFloat(s_shader_ptcSize, 32.f);
  m_wrapper.SetUniformVec3(s_shader_uColor,     {1, 1, 0});
  m_wrapper.SetUniformVec3(s_shader_inPosition, {0, 0, 0});
  m_wrapper.UseShader();

  const auto& vao_list = m_weak_vao_ref->GetVaoList();
  glBindVertexArray(vao_list[0].GetVaoId());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_sprite->Id());

  glDrawArrays(GL_POINTS, 0, 1);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component::_internal namespace