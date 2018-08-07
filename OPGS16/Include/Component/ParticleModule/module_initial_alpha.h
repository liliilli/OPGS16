#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ALPHA_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ALPHA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_alpha.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>

namespace opgs16::component {

///
/// @class CParticleModuleInitialAlpha
/// @brief Particle initial alpha setting module. (v0.1.0)
///
class CParticleModuleInitialAlpha final : public _internal::CInternalParticleModuleBase {
  struct DAlphaVariance {
    float min = 0.0f;
    float max = 1.0f;
  };

public:
  CParticleModuleInitialAlpha(CParticleEmitter& emitter);

  ///
  /// @brief Get object's initial alpha.
  ///
  float GetInitialAlpha() const noexcept;

  ///
  /// @brief Get variation offset width of alpha.
  ///
  float GetAlphaVariationOffset() const noexcept;

  ///
  /// @brief Get min and max alpha.
  /// If variation offset is 0, just return base alpha value.
  ///
  const DAlphaVariance& GetAlphaMinMax() const noexcept;

  ///
  /// @brief Set BASE initial particle alpha.
  ///
  void SetInitialAlpha(float alpha);

  ///
  /// @brief Set initial alpha variation.
  /// If you want to disable offset feature, just input 0.
  ///
  void SetAlphaVariationOffset(float alpha_offset) noexcept;

private:
  void Update(float delta_time) override final {};

  ///
  ///
  ///
  void pUpdateAlphaVariance() const noexcept;

  float m_base_alpha  = 1.0f;
  float m_alpha_offset = 0.f;
  mutable DAlphaVariance m_alpha_minmax;

  bool m_is_alpha_variable = false;

  static constexpr uint32_t hash_val = 0xFC21;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_ALPHA_H