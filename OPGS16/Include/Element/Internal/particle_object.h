#ifndef OPGS16_ELEMENT_INTERNAL_PARTICLE_OBJECT_H
#define OPGS16_ELEMENT_INTERNAL_PARTICLE_OBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_object.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-05 Create file.
///

#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16::element {
class CShaderWrapper;
} /// unnamed namespace

//!
//! Implementation
//!

namespace opgs16::element::_internal {

class CInternalParticleObject final {
public:
  ///
  ///
  ///
  void SetActivate(bool is_activate) noexcept;

  ///
  ///
  ///
  bool IsActivated() const noexcept;

  void Update(float delta_time);

  ///
  ///
  ///
  void pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const;

private:
  bool m_is_activated = false;

  // Temporary
  float m_elapsed = 0.f;
  float m_radius  = 32.f;
  opgs16::DVector3 m_local_position = {};
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PARTICLE_OBJECT_H