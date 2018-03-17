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
 * @file System/Manager/Private/physics_manager.cc
 * @brief
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 */

#include "../Public/physics_manager.h"  /*! Header file */

#include <algorithm>                    /*! std::sort */

#include "../../Components/Public/rigidbody_2d.h"        /*! opgs16::component::CRigidbody2D */
#include "../../Components/Physics2D/Collider/rectangle.h"  /*! collision::RectangleCollider2D */
#include "../Public/setting_manager.h"  /*! ::opgs16::manager::MSettingManager */

namespace opgs16 {
namespace manager {

using _internal::Item;

void MPhysicsManager::AddCollider(collision::RectangleCollider2D* const collider,
                                 component::CRigidbody2D* const rigidbody) {
    using Type = collision::RectangleCollider2D::PositionType;
    auto ld = collider->GetTipPosition(Type::LEFT_DOWN);
    auto ru = collider->GetTipPosition(Type::RIGHT_UP);

    /*! Insert to potential list */
    m_potential.emplace_back(
        std::make_unique<Item>(collider, rigidbody, ld, Item::Type::BEGIN));
    m_potential.emplace_back(
        std::make_unique<Item>(collider, rigidbody, ru, Item::Type::END));
}

void MPhysicsManager::Update() {
    /*! Sorting */
    std::sort(m_potential.begin(), m_potential.end(),
              [](const item_ptr& lhs, const item_ptr& rhs) {
        return lhs->m_position.x < rhs->m_position.x;
    });

    /*! Processing */
    for (auto& item : m_potential) {
        switch (item->m_type) {
        case Item::Type::BEGIN:
            ProceedCollisionCheck(item);
            m_active.emplace_back(item.get());
            break;
        case Item::Type::END:
            EraseItem(item);
            break;
        }
    }

    /*! Remove All potential, active list */
    Clear();
}

void MPhysicsManager::Clear() {
    m_active.clear();
    m_potential.clear();
}

void MPhysicsManager::ProceedCollisionCheck(MPhysicsManager::item_ptr& item) {
    const auto& manager = MSettingManager::Instance();
    for (auto& active_item : m_active) {
        /*! If rigidbody is same, do not check collision. */
        if (active_item->m_rigidbody == item->m_rigidbody) continue;
        if (!manager.CollisionCheck(active_item->m_collider->CollisionLayer(),
                                    item->m_collider->CollisionLayer())) continue;

        bool collision_flag = true;
        /*! AABB Collision Checking */
        using PositionType = collision::RectangleCollider2D::PositionType;
        const auto s_m = active_item->m_collider->GetTipPosition(PositionType::LEFT_DOWN);
        const auto s_M = active_item->m_collider->GetTipPosition(PositionType::RIGHT_UP);
        const auto d_m = item->m_collider->GetTipPosition(PositionType::LEFT_DOWN);
        const auto d_M = item->m_collider->GetTipPosition(PositionType::RIGHT_UP);

        if (s_M.x < d_m.x || d_M.x < s_m.x) collision_flag = false;
        else if (s_M.y < d_m.y || d_M.y < s_m.y) collision_flag = false;

        /*! If collide with each other, call specific procedure. */
        if (collision_flag) {
            const auto s_type = active_item->m_collider->CollisionType();
            const auto d_type = item->m_collider->CollisionType();

            /*! Call */
            if (s_type == collision::Collider2D::ECollisionType::COLLISION)
                active_item->m_rigidbody->OnCollisionEnter(*item->m_rigidbody);
            else
                active_item->m_rigidbody->OnTriggerEnter(*item->m_rigidbody);

            if (d_type == collision::Collider2D::ECollisionType::COLLISION)
                item->m_rigidbody->OnCollisionEnter(*active_item->m_rigidbody);
            else
                item->m_rigidbody->OnTriggerEnter(*active_item->m_rigidbody);
        }
    }
}

void MPhysicsManager::EraseItem(MPhysicsManager::item_ptr& item) {
    for (auto it = m_active.cbegin(); it != m_active.cend(); ++it) {
        if ((*it)->m_collider == item->m_collider) {
            m_active.erase(it);
            break;
        }
    }
}

} /*! opgs16::manager */
} /*! opgs16 */
