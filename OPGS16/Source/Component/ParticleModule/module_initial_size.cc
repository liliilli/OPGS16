#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_size.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_initial_size.h>
#include <Helper/Math/random.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::component {

CParticleModuleInitialSize::CParticleModuleInitialSize(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } {
}

int32_t CParticleModuleInitialSize::GetInitialSize() const noexcept {
  int32_t result_lifetime = m_base_size;

  if (m_is_size_variable) {
    result_lifetime = random::RandomIntegerRange(m_size_minmax.min, m_size_minmax.max);
  }

  return result_lifetime;
}

int32_t CParticleModuleInitialSize::GetSizeVariationOffset() const noexcept {
  return m_size_offset;
}

const CParticleModuleInitialSize::DSizeVariance&
CParticleModuleInitialSize::GetSizeMinMax() const noexcept {
  if (!m_is_size_variable) {
    pUpdateSizeMinMax();
  }

  return m_size_minmax;
}

void CParticleModuleInitialSize::SetInitialSize(int32_t size) {
  PHITOS_ASSERT(size > 0, "Base life time must be bigger than 0.");
  m_base_size = size;

  if (m_is_size_variable) {
    pUpdateSizeMinMax();
  }
}

void CParticleModuleInitialSize::SetSizeVariationOffset(int32_t size_offset) noexcept {
  PHITOS_ASSERT(size_offset > 0, "Lifetime variation offset must be bigger than 0.");
  m_size_offset = size_offset;

  if (m_size_offset == 0) {
    m_is_size_variable = false;
  }
  else {
    m_is_size_variable = true;
    pUpdateSizeMinMax();
  }
}

void CParticleModuleInitialSize::pUpdateSizeMinMax() const noexcept {
  auto new_min = m_base_size - m_size_offset;
  auto new_max = m_base_size + m_size_offset;

  if (new_min < 0) new_min = 0;
  if (new_max < 0) new_max = std::numeric_limits<int32_t>::max();

  m_size_minmax.min = new_min;
  m_size_minmax.max = new_max;
}

} /// ::opgs16::component namespace