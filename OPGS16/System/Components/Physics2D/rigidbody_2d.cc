#include "rigidbody_2d.h"                   /*! Header file */
#include "../../Manager/Public/physics_manager.h"  /*! PhysicsManager */
#include "..\..\Object\object.h"            /*! Object */

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

}