#include "rigidbody_2d.h"                   /*! Header file */
#include "..\..\Manager\physics_manager.h"  /*! PhysicsManager */
#include "..\..\Object\object.h"            /*! Object */

namespace component {

void Rigidbody2D::Update() {
    auto& physics_manager = PhysicsManager::GetInstance();
    for (auto& collider : m_colliders) {
        collider->ReflectPosition(m_bound_object.GetFinalPosition());
        physics_manager.AddCollider(collider.get(), this);
    }
}

}