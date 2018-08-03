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
#include <Component/Internal/particle_emitter.h>

namespace opgs16::component {

class CParticleRenderer final : public opgs16::component::_internal::CComponent {
  using TEmitterSmtPtr = std::unique_ptr<_internal::CParticleEmitter>;
  using TEmitterContainer = std::unordered_map<std::string, TEmitterSmtPtr>;

public:
  CParticleRenderer(element::CObject& bind_object) : CComponent{bind_object} {};
  ~CParticleRenderer();

  CParticleRenderer(const CParticleRenderer&) = default;
  CParticleRenderer(CParticleRenderer&&) = default;
  CParticleRenderer& operator=(const CParticleRenderer&) = default;
  CParticleRenderer& operator=(CParticleRenderer&&) = default;

  _internal::CParticleEmitter* CreateParticleEmitter(const std::string& emitter_name);

  _internal::CParticleEmitter* GetParticleEmitter(const std::string& emitter_name);

  bool DestroyParticleEmitter(const std::string& emitter_name);

  bool IsSleep() noexcept;

private:
  void Update(float delta_time) override final;

  TEmitterContainer m_emitter;

  bool m_is_removed_from_original_object = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CParticleRenderer)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLE_RENDERER_H