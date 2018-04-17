#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H

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
 * @file System/Manager/Internal/physics_internal.h
 * @brief
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Create file. Move PhysicsManager::Item to this file.
 */

#include "../../Components/Public/rigidbody_2d.h"   /*! opgs16::component::CRigidbody2D */

namespace opgs16 {
namespace manager {
namespace _internal {

struct Item {
    physics::CRectangleCollider2D* const    m_collider;
    component::CRigidbody2D* const   m_rigidbody;
    const glm::vec2 m_position;

    enum class Type {
        BEGIN,
        END
    } m_type = { Type::BEGIN };

    explicit Item(physics::CRectangleCollider2D* const collider,
                  component::CRigidbody2D* const rigidbody,
                  const glm::vec2 axis_value,
                  Type type) :
        m_collider{ collider }, m_rigidbody{ rigidbody },
        m_position{ axis_value }, m_type{ type } {};
};

} /*! opgs16::manager::_internal */
} /*! opgs16::manager */
} /*! opgs16::manager */

#endif // !OPGS16_SYSTEM_MANAGER_INTERNAL_PHYSICS_INTERNAL_H

