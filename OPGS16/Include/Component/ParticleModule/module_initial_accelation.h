#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ACCELATION_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ACCELATION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_accelation.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>
#include <Helper/Type/axis.h>
#include <Helper/Type/vector3.h>

namespace opgs16::component {

class CParticleModuleInitialAccelation final : public _internal::CInternalParticleModuleBase {
  struct DAccelationVariance {
    DVector3 min;
    DVector3 max;
  };

public:
  CParticleModuleInitialAccelation(CParticleEmitter& emitter);

  ///
  /// @brief Get initial accelation, or variant initial accelation from min to max.
  ///
  DVector3 GetInitialAccelation() const noexcept;

  ///
  /// @brief Set initial base accelation.
  ///
  void SetInitialAccelation(const DVector3& initial_accelation) noexcept;

  ///
  /// @brief Set accelation variant offset.
  /// If all offset value is 0, this module will just return base accelation.
  ///
  void SetAccelationVariationOffset(EAxis3D axis, float accelation_offset);

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief Update variance of initial velocity.
  ///
  void pUpdateAccelationMinMax() const noexcept;

  DVector3 m_base_accelation    = DVector3{};
  DVector3 m_accelation_offset  = DVector3{};
  mutable DAccelationVariance m_accelation_variance;

  bool m_is_accelation_variant = false;

  static constexpr uint32_t hash_val = 0x987A;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ACCELATION_H