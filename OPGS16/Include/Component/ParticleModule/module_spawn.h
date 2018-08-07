#ifndef OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SPAWN_H
#define OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SPAWN_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_spawn.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-06 Create file.
///

#include <Component/Internal/particle_module_base.h>

namespace opgs16::component {

enum class EParticleSpawnStyle {
  Batch,
  Sequence,
};

class CParticleModuleSpawn final : public _internal::CInternalParticleModuleBase {
public:
  CParticleModuleSpawn(CParticleEmitter& emitter);

  ///
  /// @brief
  ///
  void SetLoop(bool is_loop) noexcept;

  ///
  /// @brief
  ///
  void SetPeriod(int32_t period_millisecond) noexcept;

  ///
  /// @brief
  ///
  void SetSpawnNumber(int32_t spawn_number) noexcept;

  ///
  ///
  ///
  void SetSpawnStyle(EParticleSpawnStyle style) noexcept;

  ///
  ///
  ///
  bool IsLoop() const noexcept;

  ///
  ///
  ///
  int32_t GetPeriodTime() const noexcept;

  ///
  ///
  ///
  int32_t GetMaximumSpawnNumber() const noexcept;

  ///
  ///
  ///
  EParticleSpawnStyle GetSpawnStyle() const noexcept;

  ///
  ///
  ///
  void Update(float delta_time) override final;

private:
  void pUpdateSpawnObjectInterval() noexcept;

  EParticleSpawnStyle m_spawn_style = EParticleSpawnStyle::Sequence;

  int32_t m_elapsed       = 0;
  int32_t m_period_time   = 1'000;
  int32_t m_from_creation = 0;

  int32_t m_spawn_number  = 1;
  int32_t m_spawn_interval = 0;

  bool m_is_emitter_loop  = false;
  bool m_is_initiated     = false;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLEMODULE_MODULE_SPAWN_H