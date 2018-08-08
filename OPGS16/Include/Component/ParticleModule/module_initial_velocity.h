#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_VELOCITY_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_VELOCITY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_velocity.h
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

///
/// @class CParticleModuleInitialVelocity
/// @brief Particle initial (x,y,z) velocity setting module. (v0.1.0)
///
class CParticleModuleInitialVelocity final : public _internal::CInternalParticleModuleBase {
  struct DVelocityVariance {
    DVector3 min;
    DVector3 max;
  };
  OP16_SETUP_HASH(CParticleModuleInitialVelocity);
public:
  CParticleModuleInitialVelocity(CParticleEmitter& emitter);

  ///
  /// @brief Get initial velocity, or variant initial velocity from min to max.
  ///
  DVector3 GetInitialVelocity() const noexcept;

  ///
  /// @brief Set initial base velocity.
  ///
  void SetInitialVelocity(const DVector3& initial_velocity) noexcept;

  ///
  /// @brief Set velocity variant offset.
  /// If all offset value is 0, this module will just return base velocity.
  ///
  void SetVelocityVariationOffset(EAxis3D axis, float velocity_offset);

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief Update variance of initial velocity.
  ///
  void pUpdateVelocityMinMax() const noexcept;

  DVector3 m_base_velocity = DVector3{};
  DVector3 m_velocity_offset = DVector3{};
  mutable DVelocityVariance m_velocity_variance;

  bool m_is_velocity_variant = false;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_VELOCITY_H