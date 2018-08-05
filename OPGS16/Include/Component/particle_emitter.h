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
#include <Component/sprite2d_renderer.h>
#include <Component/Internal/particle_renderer.h>
#include <Element/Internal/particle_object.h>
#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CParticleSpawner;
}

//!
//! Implementation
//!

namespace opgs16::component {

class CParticleEmitter final : public _internal::CRendererBase {
  using CParticleObject = element::_internal::CInternalParticleObject;

public:
  CParticleEmitter(element::CObject& bind_object);

  ///
  /// @brief Check emitter is slept.
  /// Sleep flag will be true if emitter is not spawning any particles.
  ///
  bool IsSleep() const noexcept;

  ///
  /// @brief
  ///
  void SetTexture(const std::string& texture_name);

  ///
  /// @brief
  ///
  void SetTextureFragmentIndex(int32_t fragment_index);

  ///
  /// @brief
  ///
  void SetLoop(bool is_loop);

  ///
  /// @brief
  ///
  void Render();

private:
  ///
  ///
  ///
  void Update(float delta_time) override final;

  ///
  /// @brief
  ///
  void pfUpdateSpawnLocationBasis(const DVector3& position);

  ///
  /// @brief
  ///
  void pUpdateCommonUniformProperties(element::CShaderWrapper& wrapper);

  std::vector<CParticleObject>  m_object_list;
  _internal::CInternalParticleRenderer m_renderer;

  DVector3 m_location_basis = {};
  bool m_is_looped = false;

  friend opgs16::component::CParticleSpawner;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CRendererBase)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H