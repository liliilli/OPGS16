#ifndef OPGS16_COMPONENT_PARTICLE_RENDERER_H
#define OPGS16_COMPONENT_PARTICLE_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/particle_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
///

#include <Component/Internal/component.h>
#include <Component/particle_emitter.h>

namespace opgs16::component {

class CParticleSpawner final : public opgs16::component::_internal::CComponent {
  using TEmitterSmtPtr = std::unique_ptr<CParticleEmitter>;
  using TEmitterContainer = std::unordered_map<std::string, TEmitterSmtPtr>;

public:
  CParticleSpawner(element::CObject& bind_object) : CComponent{bind_object} {};
  ~CParticleSpawner();

  CParticleSpawner(const CParticleSpawner&) = default;
  CParticleSpawner(CParticleSpawner&&) = default;
  CParticleSpawner& operator=(const CParticleSpawner&) = default;
  CParticleSpawner& operator=(CParticleSpawner&&) = default;

  ///
  /// @brief
  ///
  CParticleEmitter* CreateEmptyParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief
  ///
  CParticleEmitter* GetParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief
  ///
  bool DestroyParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief
  ///
  bool IsSleep() noexcept;

  ///
  /// @brief
  ///
  void StartAll() noexcept;

private:
  void Update(float delta_time) override final;

  TEmitterContainer m_emitter;
  bool m_is_removed_from_original_object = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CParticleSpawner)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLE_RENDERER_H