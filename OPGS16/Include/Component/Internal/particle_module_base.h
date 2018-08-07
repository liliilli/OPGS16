#ifndef OPGS16_COMPONENT_INTERNAL_PARTICLE_MODULE_H
#define OPGS16_COMPONENT_INTERNAL_PARTICLE_MODULE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/particle_module.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-06 Create file.
///

//!
//! Forward declaration
//!

namespace opgs16::component {
class CParticleEmitter;
}

//!
//! Implementation
//!

namespace opgs16::component::_internal {

class CInternalParticleModuleBase {
public:
  CInternalParticleModuleBase(CParticleEmitter& emitter);
  virtual ~CInternalParticleModuleBase() = default;

  virtual void Update(float delta_time) = 0;

  ///
  ///
  ///
  void SetModuleActivation(bool is_activate) noexcept;

  ///
  ///
  ///
  bool IsModuleActive() const noexcept;

protected:
  CParticleEmitter& pGetEmitterRef() const noexcept;

private:
  CParticleEmitter& m_emitter_ref;

  bool m_is_active = false;
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_PARTICLE_MODULE_H