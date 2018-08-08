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
#include <Component/ParticleModule/module_lifetime.h>
#include <Component/ParticleModule/module_initial_alpha.h>
#include <Component/ParticleModule/module_initial_color.h>
#include <Component/ParticleModule/module_initial_size.h>
#include <Component/ParticleModule/module_initial_velocity.h>
#include <Component/ParticleModule/module_initial_accelation.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

namespace opgs16::component {

CParticleEmitter::CParticleEmitter(element::CObject& bind_object):
    CRendererBase{bind_object},
    m_renderer{bind_object},
    m_module_spawn{*this} {
  SetComponentActive(false);
  // @todo Temporary
  m_object_list.resize(256);
}

bool CParticleEmitter::IsSleep() const noexcept {
  if (m_valid_objects.empty())
    return true;

  return false;
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

CParticleModuleSpawn& CParticleEmitter::GetSpawnModuleRef() {
  return m_module_spawn;
}

void CParticleEmitter::pfUpdateSpawnLocationBasis(const DVector3& position) {
  m_location_basis = position;
}

void CParticleEmitter::pUpdateCommonUniformProperties(element::CShaderWrapper& wrapper) {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;

  wrapper.SetUniformMat4(builtin::s_uniform_proj, GetPresentMainCamProjMatrix());
  wrapper.SetUniformMat4(builtin::s_uniform_view, GetPresentMainCamViewMatrix());
}

void CParticleEmitter::pfCreateObjects(int32_t numbers) {
  // @todo ここでオブジェクトの初期設定をする。
  const auto obj_pool_size = static_cast<int32_t>(m_object_list.size());

  if (m_pool_last + numbers >= obj_pool_size) {
    for (int32_t i = m_pool_last; i < obj_pool_size; ++i) {
      pActivateObject(m_object_list[i]);
    }

    const int32_t remains = m_pool_last + numbers - obj_pool_size;
    for (int32_t i = 0; i < remains; ++i) {
      m_object_list[i].SetActivate(false);
      pActivateObject(m_object_list[i]);
    }

    m_pool_last = remains;
    return;
  }

  for (int32_t i = 0; i < numbers; ++i) {
    pActivateObject(m_object_list[m_pool_last + i]);
  }
  m_pool_last += numbers;
}

void CParticleEmitter::pfSetModulesActive(bool is_activate) {
  m_module_spawn.SetModuleActivation(is_activate);
}

void CParticleEmitter::pActivateObject(CParticleObject& object) {
  const auto lifetime         = GetModule<CParticleModuleLifetime>();
  const auto initial_size     = GetModule<CParticleModuleInitialSize>();
  const auto initial_color    = GetModule<CParticleModuleInitialColor>();
  const auto initial_alpha    = GetModule<CParticleModuleInitialAlpha>();
  const auto initial_velocity = GetModule<CParticleModuleInitialVelocity>();
  const auto initial_initacc  = GetModule<CParticleModuleInitialAccelation>();

  if (initial_size) { object.pfSetInitialSize(initial_size->GetInitialSize()); }
  if (lifetime) { object.pfEnableLifetime(lifetime->GetLifeTime()); }
  if (initial_color) { object.pfSetInitialColor(initial_color->GetInitialColor()); };
  if (initial_alpha) { object.pfSetInitialAlpha(initial_alpha->GetInitialAlpha()); };
  if (initial_velocity) { object.pfSetInitialVelocity(initial_velocity->GetInitialVelocity()); }
  if (initial_initacc) { object.pfSetInitialAccelation(initial_initacc->GetInitialAccelation());}

  object.pfSetInitialPosition(GetBindObject().GetFinalPosition());
  object.SetActivate(true);
  m_valid_objects.push_front(&object);
}

void CParticleEmitter::Update(float delta_time) {
  using opgs16::manager::object::InsertParticleEmitter;
  // Reorder deactivated objects.
  for (auto& element : m_valid_objects) {
    if (!element->IsActivated())
      element = nullptr;
  }
  m_valid_objects.remove_if([](auto* n) { return n == nullptr; });

  // Update common properties
  if (m_module_spawn.IsModuleActive()) {
    m_module_spawn.Update(delta_time);
  }

  // Update each objects
  for (auto& element : m_valid_objects) {
    element->Update(delta_time);
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