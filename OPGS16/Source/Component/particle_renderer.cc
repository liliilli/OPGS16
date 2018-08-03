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
#include <Component/particle_renderer.h>
#include <Headers/import_logger.h>
#include <Element/object.h>

namespace opgs16::component {

CParticleRenderer::~CParticleRenderer() {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

_internal::CParticleEmitter* CParticleRenderer::CreateParticleEmitter(const std::string& emitter_name) {
  using opgs16::component::_internal::CParticleEmitter;

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

_internal::CParticleEmitter* CParticleRenderer::GetParticleEmitter(const std::string& emitter_name) {
  if (const auto it = m_emitter.find(emitter_name); it == m_emitter.end()) {
    return nullptr;
  }
  else {
    return it->second.get();
  }
}

bool CParticleRenderer::DestroyParticleEmitter(const std::string& emitter_name) {
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

bool CParticleRenderer::IsSleep() noexcept {
  for (auto& [emitter_name, emitter_element] : m_emitter) {
    if (!emitter_element->IsSleep())
      return false;
  }

  return true;
}

void CParticleRenderer::Update(float delta_time) {
  if (m_is_removed_from_original_object)
    return;

  // Update final position as basis location.
  auto& obj = GetBindObject();
  for (auto& [emitter_name, emitter_element] : m_emitter) {
    emitter_element->pfUpdateSpawnLocationBasis(obj.GetFinalPosition());
  }
  PHITOS_NOT_IMPLEMENTED_ASSERT();
}

} /// ::opgs16::component namesapce