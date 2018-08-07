#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_velocity.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_initial_velocity.h>
#include <Helper/Math/random.h>

namespace opgs16::component {

CParticleModuleInitialVelocity::CParticleModuleInitialVelocity(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } { }

DVector3 CParticleModuleInitialVelocity::GetInitialVelocity() const noexcept {
  if (m_is_velocity_variant) {
    const auto& min = m_velocity_variance.min;
    const auto& max = m_velocity_variance.max;

    using random::RandomFloatRange;
    return DVector3{
        RandomFloatRange(min.x, max.x),
        RandomFloatRange(min.y, max.y),
        RandomFloatRange(min.z, max.z)
    };
  }

  return m_base_velocity;
}

void CParticleModuleInitialVelocity::SetInitialVelocity(const DVector3& initial_velocity) noexcept {
  m_base_velocity = initial_velocity;
  pUpdateVelocityMinMax();
}

void CParticleModuleInitialVelocity::SetVelocityVariationOffset(EAxis3D axis, float velocity_offset) {
  switch (axis) {
  case EAxis3D::X:
    m_velocity_offset.x = velocity_offset;
    break;
  case EAxis3D::Y:
    m_velocity_offset.y = velocity_offset;
    break;
  case EAxis3D::Z:
    m_velocity_offset.z = velocity_offset;
    break;
  }

  if (m_velocity_offset == DVector3{}) {
    m_is_velocity_variant = false;
  }
  else {
    m_is_velocity_variant = true;
  }

  pUpdateVelocityMinMax();
}

void CParticleModuleInitialVelocity::pUpdateVelocityMinMax() const noexcept {
  const auto min_x = m_base_velocity.x - m_velocity_offset.x;
  const auto min_y = m_base_velocity.y - m_velocity_offset.y;
  const auto min_z = m_base_velocity.z - m_velocity_offset.z;

  const auto max_x = m_base_velocity.x + m_velocity_offset.x;
  const auto max_y = m_base_velocity.y + m_velocity_offset.y;
  const auto max_z = m_base_velocity.z + m_velocity_offset.z;

  m_velocity_variance.min = DVector3{min_x, min_y, min_z};
  m_velocity_variance.max = DVector3{max_x, max_y, max_z};
}

} /// ::opgs16::component namespace