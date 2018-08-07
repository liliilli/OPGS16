#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/ParticleModule/module_spawn.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-06 Create file.
///

/// Header file
#include <Component/ParticleModule/module_spawn.h>
#include <Component/particle_emitter.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::component {

CParticleModuleSpawn::CParticleModuleSpawn(CParticleEmitter& emitter) :
    CInternalParticleModuleBase{emitter} {
  pUpdateSpawnObjectInterval();
}

void CParticleModuleSpawn::Update(float delta_time) {
  // If first time, create one.
  if (!m_is_initiated) {
    m_is_initiated = true;

    switch (m_spawn_style) {
    case EParticleSpawnStyle::Batch:

      break;
    case EParticleSpawnStyle::Sequence:
     pGetEmitterRef().pfCreateObjects(1);
      break;
    }
    return;
  }

  // If time is done, halt module or resume to first by IsLoop() return flag.
  const auto milli_time = static_cast<int32_t>(delta_time * 1'000);
  m_elapsed += milli_time;
  if (m_elapsed >= m_period_time) {
    m_elapsed -= m_period_time;

    if (!IsLoop()) {
      SetModuleActivation(false);
      return;
    }
  }

  // Activate some particle objects along with dt.
  m_from_creation += milli_time;
  if (const auto spawn_num = m_from_creation / m_spawn_interval; spawn_num > 0) {
    // @todo Emitter 에게 몇몇개의 액티베이션을 요청할 것.
    auto& emitter = pGetEmitterRef();
    emitter.pfCreateObjects(spawn_num);

    m_from_creation -= spawn_num * m_spawn_interval;
  }
}

void CParticleModuleSpawn::pUpdateSpawnObjectInterval() noexcept {
  m_spawn_interval = m_period_time / m_spawn_number;
}

void CParticleModuleSpawn::SetLoop(bool is_loop) noexcept {
  m_is_emitter_loop = is_loop;
}

void CParticleModuleSpawn::SetPeriod(int32_t period_millisecond) noexcept {
  PHITOS_ASSERT(period_millisecond > 0, "Period millisecond must be bigger than 0.");
  m_period_time = period_millisecond;
  pUpdateSpawnObjectInterval();
}

void CParticleModuleSpawn::SetSpawnNumber(int32_t spawn_number) noexcept {
  PHITOS_ASSERT(spawn_number > 0, "The number of spawn object must be bigger than 0.");
  m_spawn_number = spawn_number;
  pUpdateSpawnObjectInterval();
}

void CParticleModuleSpawn::SetSpawnStyle(EParticleSpawnStyle style) noexcept {
  m_spawn_style = style;
}

bool CParticleModuleSpawn::IsLoop() const noexcept {
  return m_is_emitter_loop;
}

int32_t CParticleModuleSpawn::GetPeriodTime() const noexcept {
  return m_period_time;
}

int32_t CParticleModuleSpawn::GetMaximumSpawnNumber() const noexcept {
  return m_spawn_number;
}

EParticleSpawnStyle CParticleModuleSpawn::GetSpawnStyle() const noexcept {
  return m_spawn_style;
}

} /// ::opgs16::component namespace