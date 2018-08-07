#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_alpha.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_initial_alpha.h>
#include <Helper/Math/random.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::component {

CParticleModuleInitialAlpha::CParticleModuleInitialAlpha(CParticleEmitter& emitter) :
    CInternalParticleModuleBase { emitter } { }

float CParticleModuleInitialAlpha::GetInitialAlpha() const noexcept {
  float result_alpha = m_base_alpha;

  if (m_is_alpha_variable) {
    result_alpha = random::RandomFloatRange(m_alpha_minmax.min, m_alpha_minmax.max);
  }

  return result_alpha;
}

float CParticleModuleInitialAlpha::GetAlphaVariationOffset() const noexcept {
  return m_alpha_offset;
}

const CParticleModuleInitialAlpha::DAlphaVariance&
CParticleModuleInitialAlpha::GetAlphaMinMax() const noexcept {
  return m_alpha_minmax;
}

void CParticleModuleInitialAlpha::SetInitialAlpha(float alpha) {
  PHITOS_ASSERT(alpha > 0, "Base life time must be bigger than 0.");
  m_base_alpha = alpha;

  if (m_is_alpha_variable) {
    pUpdateAlphaVariance();
  }
}

void CParticleModuleInitialAlpha::SetAlphaVariationOffset(float alpha_offset) noexcept {
  PHITOS_ASSERT(alpha_offset > 0, "Lifetime variation offset must be bigger than 0.");
  m_alpha_offset = alpha_offset;

  if (m_alpha_offset == 0) {
    m_is_alpha_variable = false;
  }
  else {
    m_is_alpha_variable = true;
  }

  pUpdateAlphaVariance();
}

void CParticleModuleInitialAlpha::pUpdateAlphaVariance() const noexcept {
  auto new_min = m_base_alpha - m_alpha_offset;
  auto new_max = m_base_alpha + m_alpha_offset;

  if (new_min < 0.0f) new_min = 0.0f;
  if (new_max > 1.0f) new_max = 1.0f;

  m_alpha_minmax.min = new_min;
  m_alpha_minmax.max = new_max;
}

} /// ::opgs16::component namespace