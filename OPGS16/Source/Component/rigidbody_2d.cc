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

/// @file Components/rigidbody_2d.cc
/// @author Jongmin Yun
/// @log
/// 2018-03-07 Move file to /Public, and move namespace to ::opgs16::component.
/// 2018-03-11 Corection of ::opgs16::element::CObject class.
/// 2018-03-12 Add gravity and accelation feature.

#include <Component\rigidbody_2d.h>     /// Header file

#include <Element\object.h>             /// ::opgs16::element::CObject
#include <Manager\physics_manager.h>    /// ::opgs16::manager::MPhysicsManager
#include <Manager\time_manager.h>       /// ::opgs16::manager::MTimeManager

#undef GetObject

namespace opgs16::component {

void CRigidbody2D::Update() {
    auto& object = GetObject();

    /*! Accelation */
    if (m_physics) {
        const auto delta = manager::MTimeManager::Instance().GetDeltaTime();
        m_accelation[1] = -m_gravity;
        m_velocity += m_accelation * delta;
        m_movement = m_velocity * delta;

        object.SetWorldPosition(object.GetWorldPosition() + m_movement);
    }

    /*! Collision */
    if (m_collidable) {
        auto& physics_manager = manager::MPhysicsManager::Instance();
        for (auto& collider : m_colliders) {
            collider->ReflectPosition(object.GetWorldPosition());
            physics_manager.AddCollider(collider.get(), this);
        }
    }
}

void CRigidbody2D::OnCollisionEnter(CRigidbody2D& collider) {
    GetObject().OnCollisionEnter(collider);
}

void CRigidbody2D::OnTriggerEnter(CRigidbody2D& collider) {
    GetObject().OnTriggerEnter(collider);
}

bool CRigidbody2D::IsTag(const std::string&& tag) const {
    return tag == GetObject().GetTagNameOf();
}

bool CRigidbody2D::IsTag(const size_t index) const {
    return index == GetObject().GetTagIndexOf();
}

} /*! opgs16::component */