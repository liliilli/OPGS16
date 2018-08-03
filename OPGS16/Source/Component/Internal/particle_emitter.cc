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
#include <Component/Internal/particle_emitter.h>

namespace opgs16::component::_internal {

bool CParticleEmitter::IsSleep() const noexcept {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

void CParticleEmitter::pfUpdateSpawnLocationBasis(const DVector3& position) {
  m_location_basis = position;
}

} /// ::opgs16::component::_internal namespace