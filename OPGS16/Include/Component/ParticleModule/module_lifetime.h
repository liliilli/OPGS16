#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_LIFETIME_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_LIFETIME_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_lifetime.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>

namespace opgs16::component {

///
/// @class CParticleModuleLifetime
/// @brief Particle lifetime module. (v0.1.0)
///
class CParticleModuleLifetime final : public _internal::CInternalParticleModuleBase {
  struct DTimeVariance {
    int32_t min = 0;
    int32_t max = std::numeric_limits<int32_t>::max();
  };

public:
  CParticleModuleLifetime(CParticleEmitter& emitter);

  ///
  /// @brief Get object's initial lifetime (milliseconds)
  ///
  int32_t GetLifeTime() const noexcept;

  ///
  /// @brief Get variation offset width of lifetime.
  ///
  int32_t GetLifeTimeVariationOffset() const noexcept;

  ///
  /// @brief Get min and max lifetime.
  /// If variation offset is 0, just return base lifetime value.
  ///
  const DTimeVariance& GetLifeTimeMinMax() const noexcept;

  ///
  /// @brief Set objects' initial lifetime (milliseconds)
  ///
  void SetLifeTime(int32_t milliseconds) noexcept;

  ///
  /// @brief Set lifetime variation.
  /// If you want to disable offset feature, just input 0.
  ///
  void SetLifeTimeVariationOffset(int32_t milliseconds) noexcept;

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief Just update min max lifetime of lifetime.
  ///
  void pUpdateLifeTimeMinMax() const noexcept;

  int32_t m_base_lifetime = 1'000;
  int32_t m_lifetime_offset = 0;
  mutable DTimeVariance m_lifetime_minmax;

  bool m_is_lifetime_variable = false;

  static constexpr uint32_t hash_val = 0xFF;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_LIFETIME_H