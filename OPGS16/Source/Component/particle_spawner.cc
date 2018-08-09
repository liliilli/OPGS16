#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/particle_renderer.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
///

/// Header file
#include <Component/particle_spawner.h>
#include <Headers/import_logger.h>
#include <Element/object.h>

namespace {



} /// unnamed namespace

namespace opgs16::component {

CParticleSpawner::~CParticleSpawner() {
  if (!m_is_removed_from_original_object) {

  }
  else {

  }

  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

CParticleEmitter* CParticleSpawner::CreateEmptyParticleEmitter(const std::string& emitter_name) {
  using opgs16::component::CParticleEmitter;

  if (GetParticleEmitter(emitter_name)) {
    PUSH_LOG_ERROR_EXT("Could not create particle emitter, {}.", emitter_name);
    return nullptr;
  }

  auto& obj = GetBindObject();
  auto [it, result] = m_emitter.try_emplace(emitter_name, std::make_unique<CParticleEmitter>(obj));

  if (!result) {
    PUSH_LOG_ERROR_EXT("Could not create particle emitter, {}. Unexpected Error.", emitter_name);
    PHITOS_ASSERT(result, "Could not create particle emitter. Halt program.");
    return nullptr;
  }

  return it->second.get();
}

CParticleEmitter* CParticleSpawner::GetParticleEmitter(const std::string& emitter_name) {
  if (const auto it = m_emitter.find(emitter_name); it == m_emitter.end()) {
    return nullptr;
  }
  else {
    return it->second.get();
  }
}

CParticleSpawner::TEmitterContainer&
CParticleSpawner::GetParticleEmitterList() noexcept {
  return m_emitter;
}

void CParticleSpawner::SetParticleSpawnSetting(bool is_spawn) {
  for (auto& [uid, particle] : m_emitter) {
    particle->pfSetParticleSpawnSetting(is_spawn);
  }
}

bool CParticleSpawner::DestroyParticleEmitter(const std::string& emitter_name) {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
  return false;
}

bool CParticleSpawner::IsSleep() noexcept {
  for (auto& [emitter_name, emitter_element] : m_emitter) {
    if (!emitter_element->IsSleep())
      return false;
  }

  return true;
}

void CParticleSpawner::StartAll() noexcept {
  for (auto& [emitter_name, emitter] : m_emitter) {
    emitter->SetComponentActive(true);
    emitter->pfSetParticleSpawnSetting(true);
  }
}

void CParticleSpawner::Update(float delta_time) {
  if (m_is_removed_from_original_object)
    return;

  // Update final position as basis location.
  auto& obj = GetBindObject();
  for (auto& [emitter_name, emitter_element] : m_emitter) {
    if (emitter_element->IsComponentActive()) {
      emitter_element->pfUpdateSpawnLocationBasis(obj.GetFinalPosition());
      emitter_element->Update(delta_time);
    }
  }

  //PHITOS_NOT_IMPLEMENTED_ASSERT();
}

} /// ::opgs16::component namesapce