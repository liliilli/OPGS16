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
/// 2018-08-09 Refactoring.
///

#include <Helper/Type/vector3.h>
#include <Helper/Type/color.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
struct DParticleInitialData;
class CParticleEmitter;
}

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
  /// @brief Set activate and process particle object's update and rendering.
  ///
  void SetActivate(bool is_activate) noexcept;

  ///
  /// @brief Set parent emitter raw reference.
  /// Parameter can not be nullptr.
  ///
  void SetEmitterRawReference(component::CParticleEmitter* emitter);

  ///
  /// @brief Check this particle object is activated (processed)
  ///
  bool IsActivated() const noexcept;

private:
  ///
  ///
  ///
  void Update(float delta_time);

  ///
  ///
  ///
  void pfSetInitialData(const component::DParticleInitialData& data);

  ///
  ///
  ///
  void pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const;

  component::CParticleEmitter* m_emitter_ref = nullptr;

  int32_t m_lifetime = 0;
  int32_t m_initial_size = 0;
  DColor  m_initial_color = DColor::White;
  DVector3 m_initial_velocity   = DVector3{};
  DVector3 m_initial_accelation = DVector3{};

  float   m_present_size = 0;
  float   m_present_alpha = 1.f;

  bool    m_is_activated = false;
  bool    m_is_enabled_lifetime = false;
  bool    m_is_enabled_size       = false;
  bool    m_is_enabled_sizebylife = false;
  bool    m_is_enabled_colorbylife = false;
  bool    m_is_enabled_alphabylife = false;
  bool    m_is_enabled_velocity = false;
  bool    m_is_enabled_accelation = false;

  opgs16::DVector3 m_initial_position = {};

  //!
  //! Below variables must be initialized when object is enabled.
  //!

  float m_elapsed = 0.f;
  opgs16::DVector3 m_local_position = {};

  friend opgs16::component::CParticleEmitter;
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PARTICLE_OBJECT_H