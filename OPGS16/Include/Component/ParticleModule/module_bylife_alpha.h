#ifndef OPGS16_COMPONENT_PARTICLEMODULE_BYLIFE_ALPHA_H
#define OPGS16_COMPONENT_PARTICLEMODULE_BYLIFE_ALPHA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_bylife_alpha.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-09 Create file.
///

#include <Component/Internal/particle_module_base.h>
#include <Element/Internal/particle_enum.h>
#include <Helper/Type/vector2.h>

namespace opgs16::component {

///
/// @class CParticleModuleAlphaByLife
/// @brief Particle life by alpha module. (v0.1.0)
///
class CParticleModuleAlphaByLife final : public _internal::CInternalParticleModuleBase {
  struct DControlPoint {
    DVector2 point = DVector2{0.f, 1.f};
    explicit DControlPoint(float timepoint, float alpha) :
        point{timepoint, alpha} {};

    float GetXTimepoint() const noexcept {
      return point.x;
    }

    float GetYAlpha() const noexcept {
      return point.y;
    }
  };

  OP16_SETUP_HASH(CParticleModuleAlphaByLife);
public:
  using EParticleProcessType = element::_internal::EParticleProcessType;
  CParticleModuleAlphaByLife(CParticleEmitter& emitter);

  ///
  /// @brief Get module procedure type.
  ///
  EParticleProcessType GetProcessType() const noexcept;

  ///
  /// @brief Get alpha value [0, 1] along with procedure type.
  /// If linear, just use linear interpolation but curve, use catmull-rom spline algorithm.
  ///
  float GetAlphaValue(float life_timepoint);

  ///
  /// @brief Set module procedure type. This function should be called prior to execution.
  ///
  void SetProcessType(EParticleProcessType type) noexcept;

  ///
  /// @brief Insert alpha control point to timeline which will be processed.
  ///
  /// @param[in] life_timepoint Life cycle timepoint ranges from 0 to 1 (inclusive).
  /// If value is out of bound, program will assert in debug mode.
  /// @param[in] alpha_value Alpha value. Minus value also permitted.
  ///
  void InsertControlPoint(float life_timepoint, float alpha_value);

  ///
  /// @brief Set start alpha point. default value is 1.0f
  ///
  void SetStartAlphaPoint(float scale_value);

  ///
  /// @brief Set end alpha point. default value is 1.0f
  ///
  void SetEndAlphaPoint(float scale_value);

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief
  ///
  float pProceedGetAlphaValue(float life_timepoint);

  /// Stores control point timeline.
  std::vector<DControlPoint> m_tables;
  float m_start_alpha_value = 1.f;
  float m_end_alpha_value   = 1.f;

  EParticleProcessType m_type = EParticleProcessType::Linear;
  bool m_is_control_point_sorted = false;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_BYLIFE_ALPHA_H