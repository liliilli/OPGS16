#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_accelation.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_initial_accelation.h>
#include <Helper/Math/random.h>

namespace opgs16::component {

CParticleModuleInitialAccelation::CParticleModuleInitialAccelation(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } { }

DVector3 CParticleModuleInitialAccelation::GetInitialAccelation() const noexcept {
  if (m_is_accelation_variant) {
    const auto& min = m_accelation_variance.min;
    const auto& max = m_accelation_variance.max;

    using random::RandomFloatRange;
    return DVector3{
        RandomFloatRange(min.x, max.x),
        RandomFloatRange(min.y, max.y),
        RandomFloatRange(min.z, max.z)
    };
  }

  return m_base_accelation;
}

void CParticleModuleInitialAccelation::SetInitialAccelation(const DVector3& initial_accelation) noexcept {
  m_base_accelation = initial_accelation;
  pUpdateAccelationMinMax();
}

void CParticleModuleInitialAccelation::SetAccelationVariationOffset(EAxis3D axis, float accelation_offset) {
  switch (axis) {
  case EAxis3D::X: m_accelation_offset.x = accelation_offset; break;
  case EAxis3D::Y: m_accelation_offset.y = accelation_offset; break;
  case EAxis3D::Z: m_accelation_offset.z = accelation_offset; break;
  }

  if (m_accelation_offset == DVector3{}) {
    m_is_accelation_variant = false;
  }
  else {
    m_is_accelation_variant = true;
  }

  pUpdateAccelationMinMax();
}

void CParticleModuleInitialAccelation::pUpdateAccelationMinMax() const noexcept {
  const auto min_x = m_base_accelation.x - m_accelation_offset.x;
  const auto min_y = m_base_accelation.y - m_accelation_offset.y;
  const auto min_z = m_base_accelation.z - m_accelation_offset.z;

  const auto max_x = m_base_accelation.x + m_accelation_offset.x;
  const auto max_y = m_base_accelation.y + m_accelation_offset.y;
  const auto max_z = m_base_accelation.z + m_accelation_offset.z;

  m_accelation_variance.min = DVector3{min_x, min_y, min_z};
  m_accelation_variance.max = DVector3{max_x, max_y, max_z};
}

} /// ::opgs16::component namespace