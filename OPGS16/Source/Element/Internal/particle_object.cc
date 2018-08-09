#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_object.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-05 Create file.
///

/// Header file
#include <Element/Internal/particle_object.h>

#include <Component/particle_emitter.h>
#include <Component/ParticleModule/module_bylife_scale.h>
#include <Component/ParticleModule/module_bylife_alpha.h>
#include <Shader/shader_wrapper.h>
#include "Component/ParticleModule/module_initial_alpha.h"

namespace opgs16::element::_internal {

void CInternalParticleObject::SetActivate(bool is_activate) noexcept {
  m_elapsed = 0;
  m_local_position = DVector3{};
  m_is_activated = is_activate;
}

void CInternalParticleObject::SetEmitterRawReference(component::CParticleEmitter* emitter) {
  PHITOS_ASSERT(emitter != nullptr, "Emitter reference can not be nullptr.");
  m_emitter_ref = emitter;
}

bool CInternalParticleObject::IsActivated() const noexcept {
  return m_is_activated;
}

void CInternalParticleObject::Update(float delta_time) {
  m_elapsed += delta_time * 1000;

  if (m_is_enabled_accelation) {
    m_initial_velocity += m_initial_accelation * delta_time;
  }
  if (m_is_enabled_velocity) {
    m_local_position += m_initial_velocity * delta_time;
  }

  const auto life_timepoint = static_cast<float>(m_elapsed) / m_lifetime;
  if (m_is_enabled_sizebylife) {
    auto module = m_emitter_ref->GetModule<component::CParticleModuleScaleByLife>();
    m_present_size = m_initial_size * module->GetScaleValue(life_timepoint);
  }
  if (m_is_enabled_alphabylife) {
    auto module = m_emitter_ref->GetModule<component::CParticleModuleAlphaByLife>();
    m_present_alpha = module->GetAlphaValue(life_timepoint);
  }

  if (m_is_enabled_lifetime && m_elapsed >= m_lifetime) {
    SetActivate(false);
  }
}

void CInternalParticleObject::pfSetInitialData(const component::DParticleInitialData& data) {
  m_initial_position = data.m_initial_postion;

  if (data.m_is_lifetime) {
    m_lifetime = data.m_lifetime;
    m_is_enabled_lifetime = true;
  }

  if (data.m_is_initial_size) {
    m_initial_size = data.m_initial_size;
    m_present_size = static_cast<float>(m_initial_size);
    m_is_enabled_size = true;
    if (data.m_is_bylife_size) {
      m_is_enabled_sizebylife = true;
    }
  }
  else {
    if (data.m_is_bylife_size) {
      m_initial_size = 1;
      m_present_size = 1.0f;
      m_is_enabled_size = true;
      m_is_enabled_sizebylife = true;
    }
  }

  if (data.m_is_initial_color) { m_initial_color = data.m_initial_color; }
  if (data.m_is_initial_alpha) { m_present_alpha = data.m_initial_alpha; }
  m_is_enabled_alphabylife = data.m_is_bylife_alpha;

  if (data.m_is_initial_velocity) {
    m_initial_velocity = data.m_initial_velocity;
    m_is_enabled_velocity = true;
  }

  if (data.m_is_initial_accelation) {
    m_initial_accelation = data.m_initial_accelation;
    m_is_enabled_accelation = true;
    if (!m_is_enabled_velocity) m_is_enabled_velocity = true;
  }
}

void CInternalParticleObject::pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const {
  shader_wrapper.SetUniformVec3("ptcLocalPosition", m_local_position);
  shader_wrapper.SetUniformVec3("ptcBasePosition",  m_initial_position);
  shader_wrapper.SetUniformVec3("ptcTintColor",     m_initial_color);
  shader_wrapper.SetUniformFloat("ptcAlpha",  m_present_alpha);
  shader_wrapper.SetUniformFloat("ptcSize",   m_present_size);
}

} /// ::opgs16::element::_internal namespace