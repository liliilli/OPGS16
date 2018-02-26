#include "physics_manager.h"
#include <algorithm>                /*! std::sort */
#include <utility>                  /*! std::pair */
#include "..\Components\Physics2D\Collider\rectangle.h"/*! collision::RectangleCollider2D */
#include "..\Components\Physics2D\rigidbody_2d.h"   /*! component::Rigidbody2D */

#include <iostream>

void PhysicsManager::AddCollider(collision::RectangleCollider2D* const collider,
                                 component::Rigidbody2D* const rigidbody) {
    using Type = collision::RectangleCollider2D::PositionType;
    auto ld = collider->GetTipPosition(Type::LEFT_DOWN);
    auto ru = collider->GetTipPosition(Type::RIGHT_UP);

    /*! Insert to potential list */
    m_potential.emplace_back(
        std::make_unique<Item>(collider, rigidbody, ld, Item::Type::BEGIN));
    m_potential.emplace_back(
        std::make_unique<Item>(collider, rigidbody, ru, Item::Type::END));
}

void PhysicsManager::Update() {
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

void PhysicsManager::Clear() {
    m_active.clear();
    m_potential.clear();
}

void PhysicsManager::ProceedCollisionCheck(PhysicsManager::item_ptr& item) {
    for (auto& active_item : m_active) {
        /*! If rigidbody is same, do not check collision. */
        if (active_item->m_rigidbody == item->m_rigidbody) continue;

        bool collision_flag = true;
        /*! AABB Collision Checking */
        using PositionType = collision::RectangleCollider2D::PositionType;
        auto s_m = active_item->m_collider->GetTipPosition(PositionType::LEFT_DOWN);
        auto s_M = active_item->m_collider->GetTipPosition(PositionType::RIGHT_UP);
        auto d_m = item->m_collider->GetTipPosition(PositionType::LEFT_DOWN);
        auto d_M = item->m_collider->GetTipPosition(PositionType::RIGHT_UP);

        if (s_M.x < d_m.x || d_M.x < s_m.x) collision_flag = false;
        else {
            if (s_M.y < d_m.y || d_M.y < s_m.y) collision_flag = false;
        }

        /*! If collide with each other, call specific procedure. */
        if (collision_flag) {
            auto s_type = active_item->m_collider->GetCollisionType();
            auto d_type = item->m_collider->GetCollisionType();

            /*! Call */
            if (s_type == collision::Collider2D::CollisionType::COLLISION)
                active_item->m_rigidbody->OnCollisionEnter(*item->m_rigidbody);
            else
                active_item->m_rigidbody->OnTriggerEnter(*item->m_rigidbody);

            if (d_type == collision::Collider2D::CollisionType::COLLISION)
                item->m_rigidbody->OnCollisionEnter(*active_item->m_rigidbody);
            else
                item->m_rigidbody->OnTriggerEnter(*active_item->m_rigidbody);
        }
    }
}

void PhysicsManager::EraseItem(PhysicsManager::item_ptr& item) {
    for (auto it = m_active.cbegin(); it != m_active.cend(); ++it) {
        if ((*it)->m_collider == item->m_collider) {
            m_active.erase(it);
            break;
        }
    }
}
