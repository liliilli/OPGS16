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
  m_is_activated = is_activate;
}

bool CInternalParticleObject::IsActivated() const noexcept {
  return m_is_activated;
}

void CInternalParticleObject::Update(float delta_time) {
  m_elapsed += delta_time;

  const auto x = m_radius * std::cosf(m_elapsed);
  const auto y = m_radius * std::sinf(m_elapsed);
  m_local_position.x = x;
  m_local_position.y = y;
}

void CInternalParticleObject::pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const {
  shader_wrapper.SetUniformVec3("ptcLocalPosition", m_local_position);
}

} /// ::opgs16::element::_internal namespace