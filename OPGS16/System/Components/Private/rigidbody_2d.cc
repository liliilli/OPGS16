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
 * @file System/Components/Private/rigidbody_2d.cc
 * @author Jongmin Yun
 * @log
 * 2018-03-07 Move file to /Public, and move namespace to ::opgs16::component.
 */

#include "../Public/rigidbody_2d.h"                 /*! Header file */
#include "../../Manager/Public/physics_manager.h"   /*! opgs16::manager::PhysicsManager */
#include "../../Object/object.h"                    /*! Object */

namespace opgs16 {
namespace component {

void Rigidbody2D::Update() {
    auto& physics_manager = opgs16::manager::PhysicsManager::Instance();
    for (auto& collider : m_colliders) {
        collider->ReflectPosition(m_bound_object.GetWorldPosition());
        physics_manager.AddCollider(collider.get(), this);
    }
}

void Rigidbody2D::OnCollisionEnter(Rigidbody2D& collider) {
    m_bound_object.OnCollisionEnter(collider);
}

void Rigidbody2D::OnTriggerEnter(Rigidbody2D& collider) {
    m_bound_object.OnTriggerEnter(collider);
}

bool Rigidbody2D::IsTag(const std::string&& tag) const {
    return tag == m_bound_object.GetTagNameOf();
}

bool Rigidbody2D::IsTag(const size_t index) const {
    return index == m_bound_object.GetTagIndexOf();
}

} /*! opgs16::component */
} /*! opgs16 */