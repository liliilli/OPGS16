#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SIZEBYLIFE_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SIZEBYLIFE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_sizebylife.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>
#include <Element/Internal/particle_enum.h>
#include <Helper/Type/vector2.h>

namespace opgs16::component {

///
/// @class CParticleModuleScaleByLife
/// @brief Particle life by size module. (v0.1.0)
///
class CParticleModuleScaleByLife final : public _internal::CInternalParticleModuleBase {
  struct DControlPoint {
    DVector2 point = DVector2{0.f, 1.f};
    explicit DControlPoint(float timepoint, float scale) : point{timepoint, scale} {};
    float GetXTimepoint() const noexcept {
      return point.x;
    }
    float GetYScale() const noexcept {
      return point.y;
    }
  };

  OP16_SETUP_HASH(CParticleModuleScaleByLife);
public:
  using EParticleProcessType = element::_internal::EParticleProcessType;
  CParticleModuleScaleByLife(CParticleEmitter& emitter);

  ///
  /// @brief Get module procedure type.
  ///
  EParticleProcessType GetProcessType() const noexcept;

  ///
  /// @brief Get scale value along with procedure type.
  /// If linear, just use linear interpolation but curve, use catmull-rom spline algorithm.
  ///
  float GetScaleValue(float life_timepoint);

  ///
  /// @brief Set module procedure type. This function should be called prior to execution.
  ///
  void SetProcessType(EParticleProcessType type) noexcept;

  ///
  /// @brief Insert size control point to timeline which will be processed.
  ///
  /// @param[in] life_timepoint Life cycle timepoint ranges from 0 to 1 (inclusive).
  /// If value is out of bound, program will assert in debug mode.
  /// @param[in] scale_value Scale value. Minus value also permitted.
  ///
  void InsertControlPoint(float life_timepoint, float scale_value);

  ///
  /// @brief Set start scale point. default value is 1.0f
  ///
  void SetStartScalePoint(float scale_value);

  ///
  /// @brief Set end scale point. default value is 1.0f
  ///
  void SetEndScalePoint(float scale_value);

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief
  ///
  float pProceedGetScaleValue(float life_timepoint);

  /// Stores control point timeline.
  std::vector<DControlPoint> m_tables;
  float m_start_alpha_value = 1.f;
  float m_end_alpha_value   = 1.f;

  EParticleProcessType m_type = EParticleProcessType::Linear;
  bool m_is_control_point_sorted = false;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SIZEBYLIFE_H