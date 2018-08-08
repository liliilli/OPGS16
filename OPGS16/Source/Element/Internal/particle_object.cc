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
#include <Helper/Math/math.h>
#include <Shader/shader_wrapper.h>

namespace opgs16::element::_internal {

void CInternalParticleObject::SetActivate(bool is_activate) noexcept {
  m_elapsed = 0;
  m_local_position = DVector3{};
  m_is_activated = is_activate;
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

  if (m_is_enabled_lifetime && m_elapsed >= m_lifetime) {
    SetActivate(false);
  }
}

void CInternalParticleObject::pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const {
  shader_wrapper.SetUniformVec3("ptcLocalPosition", m_local_position);
  shader_wrapper.SetUniformVec3("ptcBasePosition", m_initial_position);
  shader_wrapper.SetUniformVec3("ptcTintColor", m_initial_color);
  shader_wrapper.SetUniformFloat("ptcAlpha", m_initial_alpha);
  shader_wrapper.SetUniformFloat("ptcSize", static_cast<float>(m_initial_size));
}

} /// ::opgs16::element::_internal namespace