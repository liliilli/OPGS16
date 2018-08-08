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
#include "Helper/Type/color.h"

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
  /// @brief Set activate and process particle object's update and rendering.
  ///
  void SetActivate(bool is_activate) noexcept;

  ///
  /// @brief Check this particle object is activated (processed)
  ///
  bool IsActivated() const noexcept;

  ///
  ///
  ///
  void pUpdateLocalUniformProperties(CShaderWrapper& shader_wrapper) const;

  void Update(float delta_time);

  /// @todo TEMPORARY
  void pfEnableLifetime(int32_t time) noexcept {
    m_is_enabled_lifetime = true;
    m_lifetime = time;
  }

  /// @todo TEMPORARY
  void pfSetInitialSize(int32_t size) noexcept {
    m_is_enabled_size = true;
    m_initial_size = size;
  }

  /// @todo TEMPORARY
  void pfSetInitialColor(const DColor& color) noexcept {
    m_initial_color = color;
  }

  /// @todo TEMPORARY
  void pfSetInitialAlpha(float color) noexcept {
    m_initial_alpha = color;
  }

  /// @todo TEMPORARY
  void pfSetInitialVelocity(const DVector3& velocity) noexcept {
    m_is_enabled_velocity = true;
    m_initial_velocity    = velocity;
  }

  /// @todo TEMPORARY
  void pfSetInitialAccelation(const DVector3& accelation) noexcept {
    m_is_enabled_accelation = true;
    m_initial_accelation    = accelation;
  }

  /// @todo TEMPORARY
  void pfSetInitialPosition(const DVector3& position) noexcept {
    m_initial_position = position;
  }

private:
  bool m_is_activated = false;


  int32_t m_lifetime = 0;
  bool    m_is_enabled_lifetime = false;

  int32_t m_initial_size = 0;
  bool    m_is_enabled_size       = false;
  bool    m_is_enabled_sizebylife = false;

  DColor  m_initial_color = DColor::Black;
  bool    m_is_enabled_colorbylife = false;

  float   m_initial_alpha = 1.f;
  bool    m_is_enabled_alphabylife = false;

  DVector3 m_initial_velocity   = DVector3{};
  bool    m_is_enabled_velocity = false;

  DVector3 m_initial_accelation = DVector3{};
  bool    m_is_enabled_accelation = false;

  opgs16::DVector3 m_initial_position = {};

  //!
  //! Below variables must be initialized when object is enabled.
  //!

  float m_elapsed = 0.f;
  opgs16::DVector3 m_local_position = {};
};

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PARTICLE_OBJECT_H