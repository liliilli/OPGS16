#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/component.h
/// @author Jongmin Yun
/// @log
/// 2018-02-13 Create file and implement fundamental logic.
/// 2018-03-07 Move it from ::component to ::opgs16::component::_internal.
/// 2018-03-10 Make components bind Object explicitly.
///

#include <cstring>

#include <Phitos/Enums/activated.h>

#include <Component/Internal/component_macro.h>
#include <opgs16fwd.h>                      /// Forward declaration

namespace opgs16 {
///
/// @namespace component
/// @brief The namespace stores informations of basic OPGS16 components to be used in each object.
/// Derived user-defined component class should not exist or insert in component namespace.
///
namespace component {
namespace _internal {

///
/// @class CComponent
/// @brief The most base class of derived component classes.
///
class CComponent {
public:
  CComponent(element::CObject& bind_object) : m_object{ &bind_object } {};
  virtual ~CComponent() = 0;

  virtual void Update(float delta_time) = 0;

  ///
  /// @brief Return true/false flag whether or not your finding class is this.
  /// @param[in] type_value Hashed type value of type which you want to find.
  /// @return True/False flag, if you found proper class return true else false.
  ///
  inline virtual bool DoesTypeMatch(
      const uint32_t type_value,
      const char* str) const noexcept {
    return type == type_value && strcmp(str, __string_literal) == 0;
  }

  ///
  /// @brief
  /// Get reference of bound object.
  /// This function will replace previous GetObject member function,
  /// because GetObject function name is collided with Windows macro GetObjectW.
  ///
  /// @return Reference of bound game object.
  ///
  element::CObject& GetBindObject() const {
    return *m_object;
  }

  inline bool IsComponentActive() const noexcept {
    return static_cast<bool>(m_activated);
  }

  inline void SetComponentActive(bool is_activate) noexcept {
    m_activated = static_cast<phitos::enums::EActivated>(is_activate);
  }

private:
  /// Bound object which script instance refers to.
  element::CObject* m_object = nullptr;
  phitos::enums::EActivated m_activated = phitos::enums::EActivated::Activated;

  SET_UP_HASH_VALUE(CComponent)
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H
