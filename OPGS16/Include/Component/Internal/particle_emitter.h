#ifndef OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H
#define OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_emitter.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
///

#include <Component/Internal/renderer_base.h>
#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CParticleRenderer;
}

//!
//! Implementation
//!

namespace opgs16::component::_internal {

class CParticleEmitter final : public CRendererBase {
public:
  CParticleEmitter(element::CObject& bind_object) : CRendererBase{bind_object} {};

  ///
  /// @brief Check emitter is slept.
  /// Sleep flag will be true if emitter is not spawning any particles.
  ///
  bool IsSleep() const noexcept;

private:
  ///
  /// @brief
  ///
  void pfUpdateSpawnLocationBasis(const DVector3& position);

  opgs16::DVector3 m_location_basis = {};

  /// Shader is in ShaderManager, render sprite.
  element::CShaderWrapper m_wrapper;
  /// Quad VAO to render sprite on screen.
  element::CVaoContainer* m_weak_vao_ref = nullptr;

  friend opgs16::component::CParticleRenderer;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRendererBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H