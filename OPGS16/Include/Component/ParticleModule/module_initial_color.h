#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_COLOR_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_COLOR_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_color.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>
#include <Helper/Type/color.h>

namespace opgs16::component {
///
/// @class CParticleModuleInitialColor
/// @brief
///
class CParticleModuleInitialColor final : public _internal::CInternalParticleModuleBase {
  struct DColorVariance {
    DColor min = DColor::Black;
    DColor max = DColor::White;
  };

public:
  CParticleModuleInitialColor(CParticleEmitter& emitter);

  ///
  /// @brief Get initial or variant initial color (r, g, b).
  /// Alpha channel is not used.
  ///
  opgs16::DColor GetInitialColor() const noexcept;

  ///
  /// @brief Get min and max color.
  /// If variation offset is 0, just return base initial color value.
  ///
  const DColorVariance& GetColorMinMax() const noexcept;

  ///
  /// @brief Set initial base color (r, g, b).
  /// Alpha channel is not used.
  ///
  void SetInitialColor(const DColor& initial_color) noexcept;

  ///
  /// @brief Set initial color variation.
  /// If you want to disable offset feature, just input opgs16::DColor::Black.
  ///
  void SetColorVariationOffset(const DColor& color_variance) noexcept;

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief Update variance of initial color.
  ///
  void pUpdateColorMinMax() const noexcept;

  DColor m_color = DColor::White;
  DColor m_color_offset;
  mutable DColorVariance m_color_minmax;

  bool m_is_color_variable = false;

  static constexpr uint32_t hash_val = 0xADFF;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_COLOR_H