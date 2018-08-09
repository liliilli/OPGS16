#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_bylife_alpha.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-09 Create file.
///

/// Header file
#include <Component/ParticleModule/module_bylife_alpha.h>
#include <glm/gtx/spline.hpp>
#include <Helper/Math/math.h>

namespace opgs16::component {

CParticleModuleAlphaByLife::CParticleModuleAlphaByLife(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{ emitter } {}

CParticleModuleAlphaByLife::EParticleProcessType
CParticleModuleAlphaByLife::GetProcessType() const noexcept {
  return m_type;
}

float CParticleModuleAlphaByLife::GetAlphaValue(float life_timepoint) {
  if (!m_is_control_point_sorted) {
    InsertControlPoint(0.0f, m_start_alpha_value);
    InsertControlPoint(1.0f, m_end_alpha_value);

    if (!m_tables.empty()) {
      std::sort(m_tables.begin(), m_tables.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.GetXTimepoint() < rhs.GetXTimepoint();
      });
    }
    m_is_control_point_sorted = true;
  }

  return pProceedGetAlphaValue(life_timepoint);
}

void CParticleModuleAlphaByLife::SetProcessType(EParticleProcessType type) noexcept {
  m_type = type;
}

void CParticleModuleAlphaByLife::InsertControlPoint(float life_timepoint,
                                                    float alpha_value) {
  auto it = std::find_if(m_tables.begin(), m_tables.end(), [life_timepoint](const auto& element) {
    using opgs16::math::IsNearlyEqual;
    return IsNearlyEqual(element.point.x, life_timepoint);
  });
  if (it != m_tables.end()) {
    it->point.x = life_timepoint;
    it->point.y = alpha_value;
    return;
  }

  m_tables.emplace_back(DControlPoint{life_timepoint, alpha_value});
}

void CParticleModuleAlphaByLife::SetStartAlphaPoint(float scale_value) {
  m_start_alpha_value = scale_value;
}

void CParticleModuleAlphaByLife::SetEndAlphaPoint(float scale_value) {
  m_end_alpha_value = scale_value;
}

float CParticleModuleAlphaByLife::pProceedGetAlphaValue(float life_timepoint) {
  PHITOS_ASSERT(life_timepoint >= 0.f, "Input life timepoint must be bigger than or equal to 0.");

  auto it = std::find_if(
      m_tables.begin(), m_tables.end(),
      [life_timepoint](const auto& element) {
    return life_timepoint < element.GetXTimepoint();
  });

  float alpha_value;
  if (it == m_tables.end()) {
    alpha_value = m_tables[m_tables.size() - 1].GetYAlpha();
  }
  else {
    const auto length = it->point.x - (it - 1)->point.x;
    const auto offset = (life_timepoint - (it - 1)->point.x) / length;

    switch (m_type) {
    default: alpha_value = 1.f; break;
    case EParticleProcessType::Linear:
      alpha_value = math::Lerp((it - 1)->point.y, it->point.y, offset);
      break;
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

      const auto scale = glm::catmullRom<glm::vec2>(dummy_start, (it - 1)->point, it->point, dummy_end, offset);
      alpha_value = scale.y;
      break;
    }
  }

  if (alpha_value > 1.f) alpha_value = 1.f;
  if (alpha_value < 0.f) alpha_value = 0.f;
  return alpha_value;
};

} /// ::opgs16::component namespace