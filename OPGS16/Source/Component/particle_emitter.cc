#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_emitter.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
///

/// Header file
#include <Component/particle_emitter.h>
#include <Manager/scene_manager.h>
#include "Manager/Internal/shader_builtin_keywords.h"

namespace opgs16::component {

CParticleEmitter::CParticleEmitter(element::CObject& bind_object):
    CRendererBase{bind_object},
    m_renderer{bind_object} {
  SetComponentActive(false);

  // @todo Temporary
  m_object_list.resize(1);
  m_object_list[0].SetActivate(true);

}

bool CParticleEmitter::IsSleep() const noexcept {
  for (const auto& element : m_object_list) {
    if (element.IsActivated())
      return false;
  }

  return true;
}

void CParticleEmitter::SetTexture(const std::string& texture_name) {
  m_renderer.SetTexture(texture_name);
}

void CParticleEmitter::SetTextureFragmentIndex(int32_t fragment_index) {
  m_renderer.SetTextureFragmentIndex(fragment_index);
}

void CParticleEmitter::SetLoop(bool is_loop) {
  m_is_looped = is_loop;
}

void CParticleEmitter::pfUpdateSpawnLocationBasis(const DVector3& position) {
  m_location_basis = position;
}

void CParticleEmitter::pUpdateCommonUniformProperties(element::CShaderWrapper& wrapper) {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());
  wrapper.SetUniformFloat("ptcSize", 32.f);
  wrapper.SetUniformFloat("ptcAlpha", 0.5f);
  wrapper.SetUniformVec3("ptcTintColor",     {1, 1, 0});
  wrapper.SetUniformVec3("ptcBasePosition", {0, 0, 0});
}

void CParticleEmitter::Update(float delta_time) {
  using opgs16::manager::object::InsertParticleEmitter;
  // Reorder deactivated objects.


  // Update common properties


  // Update each objects
  for (auto& element : m_object_list) {
    if (!element.IsActivated())
      continue;

    element.Update(delta_time);
  }

  // Do not reorder this.
  InsertParticleEmitter(*this);
}

void CParticleEmitter::Render() {
  // 1. 공통 성질을 바인딩한다.
  auto& wrapper = m_renderer.GetShaderWrapper();
  pUpdateCommonUniformProperties(wrapper);

  for (auto& element : m_object_list) {
    if (!element.IsActivated())
      continue;

    // 3. object 마다 각기 다른 성질을 바인딩한다.
    element.pUpdateLocalUniformProperties(m_renderer.GetShaderWrapper());
    // 2. 쉐이더를 켠다.
    // 4. object 하나씩 렌더링한다.
    m_renderer.Render();
  }
}

} /// ::opgs16::component namespace