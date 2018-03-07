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
 * @file System/Components/Internal/component.h
 * @author Jongmin Yun
 * @log
 * 2018-02-13 Create file and implement fundamental logic.
 * 2018-03-07 Move it from ::component to ::opgs16::component::_internal.
 */

#include "component_macro.h"    /*! SET_UP_HASH_VALUE(__TYPE__) */

namespace opgs16 {
/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {
namespace _internal {

/*!
 * @class Component
 * @brief The most base class of derived component classes.
 */
class Component {
public:
    virtual ~Component() = default;

    virtual void Update() = 0;

    /*!
     * @brief Return true/false flag whether or not your finding class is this.
     * @param[in] type_value Hashed type value of type which you want to find.
     * @return True/False flag, if you found proper class return true else false.
     */
    inline virtual bool DoesTypeMatch(const size_t type_value) const noexcept {
        return type == type_value;
    }

    SET_UP_HASH_VALUE(Component)
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_COMPONENT_H
