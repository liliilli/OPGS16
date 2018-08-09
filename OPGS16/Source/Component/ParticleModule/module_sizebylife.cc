#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_sizebylife.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

/// Header file
#include <Component/ParticleModule/module_sizebylife.h>
#include <glm/gtx/spline.hpp>
#include <Helper/Math/math.h>

namespace opgs16::component {

CParticleModuleSizeByLife::CParticleModuleSizeByLife(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } {}

CParticleModuleSizeByLife::EParticleProcessType
CParticleModuleSizeByLife::GetProcessType() const noexcept {
  return m_type;
}

float CParticleModuleSizeByLife::GetScaleValue(float life_timepoint) {
  if (!m_is_control_point_sorted) {
    InsertControlPoint(0.0f, m_start_scale_value);
    InsertControlPoint(1.0f, m_end_scale_value);

    if (!m_tables.empty()) {
      std::sort(m_tables.begin(), m_tables.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.GetXTimepoint() < rhs.GetXTimepoint();
      });
    }
    m_is_control_point_sorted = true;
  }

  return pProceedGetScaleValue(life_timepoint);
}

void CParticleModuleSizeByLife::SetProcessType(EParticleProcessType type) noexcept {
  m_type = type;
}

void CParticleModuleSizeByLife::InsertControlPoint(float life_timepoint,
                                                   float scale_value) {
  auto it = std::find_if(m_tables.begin(), m_tables.end(), [life_timepoint](const auto& element) {
    using opgs16::math::IsNearlyEqual;
    return IsNearlyEqual(element.point.x, life_timepoint);
  });
  if (it != m_tables.end()) {
    it->point.x = life_timepoint;
    it->point.y = scale_value;
    return;
  }

  m_tables.emplace_back(DControlPoint{life_timepoint, scale_value});
}

void CParticleModuleSizeByLife::SetStartScalePoint(float scale_value) {
  m_start_scale_value = scale_value;
}

void CParticleModuleSizeByLife::SetEndScalePoint(float scale_value) {
  m_end_scale_value = scale_value;
}

float CParticleModuleSizeByLife::pProceedGetScaleValue(float life_timepoint) {
  PHITOS_ASSERT(life_timepoint >= 0.f, "Input life timepoint must be bigger than or equal to 0.");

  auto it = std::find_if(
      m_tables.begin(), m_tables.end(),
      [life_timepoint](const auto& element) {
    return life_timepoint < element.GetXTimepoint();
  });

  if (it == m_tables.end()) {
    return m_tables[m_tables.size() - 1].GetYScale();
  }

  const auto length = it->point.x - (it-1)->point.x;
  const auto offset = (life_timepoint - (it-1)->point.x) / length;

  switch (m_type) {
  default: return 1.f;
  case EParticleProcessType::Linear:
    return (it-1)->point.y * (1 - offset) + it->point.y * offset;
  case EParticleProcessType::Curve:
    DVector2 dummy_start;
    DVector2 dummy_end;

    if (auto prev_it = it - 1; prev_it == m_tables.begin())
      dummy_start = prev_it->point;
    else
      dummy_start = (--prev_it)->point;

    if (const auto next_it = it + 1; next_it == m_tables.end())
      dummy_end = it->point;
    else
      dummy_end = next_it->point;

    const auto scale_value = glm::catmullRom<glm::vec2>(
        dummy_start,
        (it - 1)->point,
        it->point,
        dummy_end,
        offset);
    return scale_value.y;
  }
};

} /// ::opgs16::component namespace