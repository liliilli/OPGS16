#ifndef OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
#define OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/script_frame.h
///
/// @brief Base script frame file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-13 Create file and make basic features.
/// 2018-03-07 Move it from ::component to ::opgs16::component.
/// 2018-05-24 Implement life cycle and assertion in case of violations.
///

#include <string>

#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h>
#include <Component/Internal/script_frame_flag.h>

//!
//! Forward declaration
//!

namespace opgs16::element {
class CObject;
}

namespace opgs16::component::_internal {
class CColliderBase;
}

//!
//! Implementation
//!

namespace opgs16::component {

///
/// @class CScriptFrame
/// @brief
/// The most base script class, which is derived by user-defined script class.
/// ScriptFrame inherits component::CComponent class the most base class of
/// all components.
///
/// All derived class must be a final,
/// and define Initiate(), Start() and Update() function and many callbacks.
///
/// Initiate() and Start() will be called only once in initialization time,
/// and prior to first Update() call.
///
class CScriptFrame : public _internal::CComponent {
public:
  using CCollider = opgs16::component::_internal::CColliderBase;

  friend class element::CObject;

public:
  CScriptFrame(element::CObject& bound_obj) : CComponent{ bound_obj } {}

  ///
  /// @brief
  /// This method will be called only one time, when ScriptFrame is attached to
  /// object, and actived first time in game applicaiton.
  ///
  /// This is just optional, you can do not to override Initiate() method
  /// when there is no need.
  ///
  virtual void Initiate() {};

  ///
  /// @brief
  /// This method must be called in Initialization time of script bound to object.
  /// And all derived scripts have to override this,
  /// even though not have components to set up.
  ///
  virtual void Start() {};

  ///
  /// @brief
  /// This method updates information of script instance bound to specific object.
  /// Must be overriden and implemented by derived script class.
  ///
  void Update(float delta_time) override {};

  ///
  /// @brief
  /// This method will be called only one time, when the object are destroyed.
  /// Object's destuctor call all destroy() methods in all scripts bound to object to be vanished.
  /// This is optional method, you could leave it vacant.
  ///
  virtual void Destroy() {};

  ///
  /// @brief
  /// This method will be called
  /// if only bound object is activated from deactivation.
  ///
  virtual void OnEnabled() {};

  ///
  /// @brief
  /// This method will be called
  /// If only bound object is desabled from activation.
  ///
  virtual void OnDisabled() {};

  ///
  /// @brief Callback function of collider enter but both are collidable.
  ///
  virtual void OnCollisionEnter(CCollider* collider) {};

  ///
  /// @brief Callback function of collider stay but both are collidable.
  ///
  virtual void OnCollisionStay(CCollider* collider) {};

  ///
  /// @brief Callback function of collider exit but both are collidable.
  ///
  virtual void OnCollisionExit(CCollider* collider) {};

  ///
  /// @brief Callback function of trigger enter.
  ///
  virtual void OnTriggerEnter(CCollider* collider) {};

  ///
  /// @brief Callback function of trigger stay.
  ///
  virtual void OnTriggerStay(CCollider* collider) {};

  ///
  /// @brief Callback function of trigger exit.
  ///
  virtual void OnTriggerExit(CCollider* collider) {};

private:
  /// internal flag for Initiate() funciton.
  mutable _internal::EScriptStarted m_started = _internal::EScriptStarted::NotStarted;

  /// Name must be same as ScriptFrame derived.
  const std::string m_script_name;

  /// Create members related to type hash value.
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CScriptFrame)
};

} /// ::opgs16::component

#define OP16_SCRIPT_GENERATE(__MAClassName__) \
  __MAClassName__(opgs16::element::CObject& bind_object) : \
      CScriptFrame{bind_object} {}; \
  SET_UP_TYPE_MEMBER(::opgs16::component::CScriptFrame, __MAClassName__)

#endif /// OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
