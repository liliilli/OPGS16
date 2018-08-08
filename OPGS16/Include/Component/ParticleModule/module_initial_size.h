#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_SIZE_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_SIZE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_initial_size.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-07 Create file.
///

#include <Component/Internal/particle_module_base.h>

namespace opgs16::component {

///
/// @class CParticleModuleInitialSize
/// @brief Particle initial size setting module. (v0.1.0)
///
class CParticleModuleInitialSize final : public _internal::CInternalParticleModuleBase {
  struct DSizeVariance {
    int32_t min = 0;
    int32_t max = std::numeric_limits<decltype(max)>::max();
  };
  OP16_SETUP_HASH(CParticleModuleInitialSize);
public:
  CParticleModuleInitialSize(CParticleEmitter& emitter);

  ///
  /// @brief Get object's initial size (unit)
  ///
  int32_t GetInitialSize() const noexcept;

  ///
  /// @brief Get variation offset width of size.
  ///
  int32_t GetSizeVariationOffset() const noexcept;

  ///
  /// @brief Get min and max size.
  /// If variation offset is 0, just return base size value.
  ///
  const DSizeVariance& GetSizeMinMax() const noexcept;

  ///
  /// @brief Set BASE initial particle size.
  ///
  void SetInitialSize(int32_t size);

  ///
  /// @brief Set initial size variation.
  /// If you want to disable offset feature, just input 0.
  ///
  void SetSizeVariationOffset(int32_t size_offset) noexcept;

private:
  void Update(float delta_time) override final {};

  ///
  /// @brief Just update min max size of size.
  ///
  void pUpdateSizeMinMax() const noexcept;

  int32_t m_base_size   = 1'000;
  int32_t m_size_offset = 0;
  mutable DSizeVariance m_size_minmax;

  bool m_is_size_variable = false;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_INITIAL_SIZE_H