#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_lifetime.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_lifetime.h>
#include <Helper/Math/random.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::component {

CParticleModuleLifetime::CParticleModuleLifetime(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } {
}

int32_t CParticleModuleLifetime::GetLifeTime() const noexcept {
  int32_t result_lifetime = m_base_lifetime;

  if (m_is_lifetime_variable) {
    result_lifetime = random::RandomIntegerRange(m_lifetime_minmax.min, m_lifetime_minmax.max);
  }

  return result_lifetime;
}

int32_t CParticleModuleLifetime::GetLifeTimeVariationOffset() const noexcept {
  return m_lifetime_offset;
}

const CParticleModuleLifetime::DTimeVariance&
CParticleModuleLifetime::GetLifeTimeMinMax() const noexcept {
  if (m_is_lifetime_variable) {
    return m_lifetime_minmax;
  }

  return {m_base_lifetime, m_base_lifetime};
}

void CParticleModuleLifetime::SetLifeTime(int32_t milliseconds) noexcept {
  PHITOS_ASSERT(milliseconds > 0, "Base life time must be bigger than 0.");
  m_base_lifetime = milliseconds;

  if (m_is_lifetime_variable) {
    pUpdateLifeTimeMinMax();
  }
}

void CParticleModuleLifetime::SetLifeTimeVariationOffset(int32_t milliseconds) noexcept {
  PHITOS_ASSERT(milliseconds > 0, "Lifetime variation offset must be bigger than 0.");
  m_lifetime_offset = milliseconds;

  if (m_lifetime_offset == 0) {
    m_is_lifetime_variable = false;
  }
  else {
    m_is_lifetime_variable = true;
    pUpdateLifeTimeMinMax();
  }
}

void CParticleModuleLifetime::pUpdateLifeTimeMinMax() noexcept {
  auto new_min = m_base_lifetime - m_lifetime_offset;
  auto new_max = m_base_lifetime + m_lifetime_offset;

  if (new_min < 0) new_min = 0;
  if (new_max < 0) new_max = std::numeric_limits<int32_t>::max();

  m_lifetime_minmax.min = new_min;
  m_lifetime_minmax.max = new_max;
}

} /// ::opgs16::component namespace