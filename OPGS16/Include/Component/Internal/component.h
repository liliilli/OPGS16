#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file Components/Internal/component.h
 * @author Jongmin Yun
 * @log
 * 2018-02-13 Create file and implement fundamental logic.
 * 2018-03-07 Move it from ::component to ::opgs16::component::_internal.
 * 2018-03-10 Make components bind Object explicitly.
 */

#include <opgs16fwd.h>                      /// Forward declaration
#include <Phitos/Enums/activated.h>
#include <Component\Internal\component_macro.h>

namespace opgs16 {
/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {
namespace _internal {

/*!
 * @class CComponent
 * @brief The most base class of derived component classes.
 */
class CComponent {
public:
    CComponent(element::CObject& bind_object) : m_object{ bind_object } {};
    virtual ~CComponent() = default;

    virtual void Update(float delta_time) = 0;

    /*!
     * @brief Return true/false flag whether or not your finding class is this.
     * @param[in] type_value Hashed type value of type which you want to find.
     * @return True/False flag, if you found proper class return true else false.
     */
    inline virtual bool DoesTypeMatch(const uint32_t type_value) const noexcept {
        return type == type_value;
    }

    /*! Get Object reference */
    element::CObject& GetObject() const {
        return m_object;
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
    return m_object;
  }

  inline phitos::enums::EActivated IsComponentActivated() const noexcept {
    return m_activated;
  }

  inline void SetComponentActivation(phitos::enums::EActivated value) noexcept {
    m_activated = value;
  }

private:
  element::CObject& m_object;   /*! Bound object which script instance refers to */
  phitos::enums::EActivated m_activated = phitos::enums::EActivated::Activated;

  SET_UP_HASH_VALUE(Component)
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H
