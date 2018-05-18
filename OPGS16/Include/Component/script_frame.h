#ifndef OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
#define OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H

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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Components\script_frame.h
 * @brief Base script frame file.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-13 Create file and make basic features.
 * 2018-03-07 Move it from ::component to ::opgs16::component.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <string>                           

/// ::opgs16::component::_internal::CComponent
#include <Component\Internal\component.h>
#include <Component\Internal\component_macro.h> /// Component macroes

#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
namespace component {

/*!
 * @class CScriptFrame
 * @brief The most base script class, which is derived by all user-defined script class.
 * ScriptFrame inherits component::CComponent class the most base class of all components.
 *
 * All derived class must be a final, and define Start() and Update() member function.
 * Initiate() and Start() must be called only once in initialization time.
 */
class CScriptFrame : public _internal::CComponent {
public:
    CScriptFrame(element::CObject& bound_obj) : _internal::CComponent{ bound_obj } {}

    /*!
     * @brief This method will be called only one time, when ScriptFrame is attached to object,
     * and actived first time in game applicaiton.
     * This is just optional, you can do not to override Initiate() method when there is no need.
     */
     virtual void Initiate() {};

    /*!
     * @brief This method must be called in Initialization time of script bound to object.
     * And all derived scripts have to override this, even though not have components to set up.
     */
     virtual void Start() = 0;

    /*!
     * @brief This method updates information of script instance bound to specific object.
     * Must be overriden and implemented by derived script class.
     */
    // virtual void Update() = 0;

    /*!
     * @brief This method will be called only one time, when the object are destroyed.
     * Object's destuctor call all destroy() methods in all scripts bound to object to be vanished.
     * This is optional method, you could leave it vacant.
     */
     virtual void Destroy() {};

private:
    const std::string m_script_name;    /*! Name must be same as ScriptFrame derived. */

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CScriptFrame)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_SCRIPT_FRAME_H
