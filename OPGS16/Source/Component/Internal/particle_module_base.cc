#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/particle_module_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-06 Create file.
///

/// Header file
#include <Component/Internal/particle_module_base.h>

namespace opgs16::component::_internal {

CInternalParticleModuleBase::CInternalParticleModuleBase(CParticleEmitter& emitter) :
    m_emitter_ref{emitter} { }

void CInternalParticleModuleBase::SetModuleActivation(bool is_activate) noexcept {
  m_is_active = is_activate;
}

bool CInternalParticleModuleBase::IsModuleActive() const noexcept {
  return m_is_active;
}

  CParticleEmitter& CInternalParticleModuleBase::pGetEmitterRef() const noexcept {
  return m_emitter_ref;
}

} /// ::opgs16::component::_internal namespace