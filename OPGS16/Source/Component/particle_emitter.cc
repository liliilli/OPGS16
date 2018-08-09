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
#include <Component/ParticleModule/module_bylife_scale.h>
#include <Component/ParticleModule/module_bylife_alpha.h>
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
  // @todo ª³ª³ªÇ«ª«Ö«¸«§«¯«ÈªÎôøÑ¢àâïÒªòª¹ªë¡£
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

void CParticleEmitter::pfSetParticleSpawnSetting(bool is_spawn) {
  m_module_spawn.SetModuleActivation(is_spawn);
}

void CParticleEmitter::pActivateObject(CParticleObject& object) {
  const auto lifetime         = GetModule<CParticleModuleLifetime>();
  const auto initial_size     = GetModule<CParticleModuleInitialSize>();
  const auto initial_color    = GetModule<CParticleModuleInitialColor>();
  const auto initial_alpha    = GetModule<CParticleModuleInitialAlpha>();
  const auto initial_velocity = GetModule<CParticleModuleInitialVelocity>();
  const auto initial_initacc  = GetModule<CParticleModuleInitialAccelation>();

  DParticleInitialData data;
  data.m_initial_postion = GetBindObject().GetFinalPosition();

  if (initial_size) {
    data.m_initial_size    = initial_size->GetInitialSize();
    data.m_is_initial_size = true;
  }
  if (lifetime) {
    data.m_lifetime    = lifetime->GetLifeTime();
    data.m_is_lifetime = true;
  }
  if (initial_color) {
    data.m_initial_color    = initial_color->GetInitialColor();
    data.m_is_initial_color = true;
  }
  if (initial_alpha) {
    data.m_initial_alpha    = initial_alpha->GetInitialAlpha();
    data.m_is_initial_alpha = true;
  }
  if (initial_velocity) {
    data.m_initial_velocity     = initial_velocity->GetInitialVelocity();
    data.m_is_initial_velocity  = true;
  }
  if (initial_initacc) {
    data.m_initial_accelation   = initial_initacc->GetInitialAccelation();
    data.m_is_initial_accelation= true;
  }
  if (GetModule<CParticleModuleScaleByLife>()) {
    data.m_is_bylife_size = true;
  }
  if (GetModule<CParticleModuleAlphaByLife>()) {
    data.m_is_bylife_alpha = true;
  }

  object.pfSetInitialData(data);
  object.SetEmitterRawReference(this);
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
  // 1. °øÅë ¼ºÁúÀ» ¹ÙÀÎµùÇÑ´Ù.
  auto& wrapper = m_renderer.GetShaderWrapper();
  pUpdateCommonUniformProperties(wrapper);

  for (auto& element : m_valid_objects) {
    if (!element->IsActivated())
      continue;

    element->pUpdateLocalUniformProperties(m_renderer.GetShaderWrapper());
    m_renderer.Render();
  }
}

} /// ::opgs16::component namespace