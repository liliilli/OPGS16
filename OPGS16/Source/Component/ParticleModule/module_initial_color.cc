#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_color.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_initial_color.h>
#include <Helper/Math/random.h>

namespace opgs16::component {

CParticleModuleInitialColor::CParticleModuleInitialColor(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } {}

opgs16::DColor CParticleModuleInitialColor::GetInitialColor() const noexcept {
  if (m_is_color_variable) {
    const auto& min = m_color_minmax.min;
    const auto& max = m_color_minmax.max;

    using random::RandomFloatRange;
    return DColor{ RandomFloatRange(min.r, max.r),
                   RandomFloatRange(min.g, max.g),
                   RandomFloatRange(min.b, max.b)
    };
  }

  return m_color;
}

const CParticleModuleInitialColor::DColorVariance&
CParticleModuleInitialColor::GetColorMinMax() const noexcept {
  return m_color_minmax;
}

void CParticleModuleInitialColor::SetInitialColor(const DColor& initial_color) noexcept {
  m_color = initial_color;
}

void CParticleModuleInitialColor::SetColorVariationOffset(const DColor& color_variance) noexcept {
  if (color_variance.r == DColor::Black.r &&
      color_variance.g == DColor::Black.g &&
      color_variance.b == DColor::Black.b) {
    m_is_color_variable = false;
  }
  else {
    m_is_color_variable = true;
  }

  m_color_offset = color_variance;
  pUpdateColorMinMax();
}

void CParticleModuleInitialColor::pUpdateColorMinMax() const noexcept {
  auto min_r = m_color.r - m_color_offset.r;
  auto min_g = m_color.g - m_color_offset.g;
  auto min_b = m_color.b - m_color_offset.b;

  auto max_r = m_color.r + m_color_offset.r;
  auto max_g = m_color.g + m_color_offset.g;
  auto max_b = m_color.b + m_color_offset.b;

  if (min_r < 0) min_r = 0;
  if (min_g < 0) min_g = 0;
  if (min_b < 0) min_b = 0;

  if (max_r > 1.f) max_r = 1.f;
  if (max_g > 1.f) max_g = 1.f;
  if (max_b > 1.f) max_b = 1.f;

  m_color_minmax.min = DColor{min_r, min_g, min_b};
  m_color_minmax.max = DColor{max_r, max_g, max_b};
};

} /// ::opgs16::component namespace