#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Components/rigidbody_2d.cc
///
/// @brief 2D Rigidbody component.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-07 Move file to /Public, and move namespace to ::opgs16::component.
/// 2018-03-11 Corection of ::opgs16::element::CObject class.
/// 2018-03-12 Add gravity and accelation feature.
/// 2018-06-01 Reflect change of opgs16::manager::physics (former MPhysicsManager0
///

/// Header file
#include <Component/rigidbody_2d.h>

/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::manager::MPhysicsManager
#include <Manager/physics_manager.h>
/// ::opgs16::manager::MTimeManager
#include <Manager/time_manager.h>

namespace opgs16::component {

void CRigidbody2D::Update() {
  auto& object = GetBindObject();

  // Accelation
  if (m_physics) {
    const auto delta = manager::time::GetDeltaTime();
    m_accelation[1] = -m_gravity;
    m_velocity += m_accelation * delta;
    m_movement = m_velocity * delta;

    object.SetWorldPosition(object.GetWorldPosition() + DVector3{m_movement});
  }

  // Collision
  if (m_collidable) {
    for (auto& collider : m_colliders) {
      collider->ReflectPosition(static_cast<glm::vec3>(object.GetWorldPosition()));
      manager::physics::AddCollider(collider.get(), this);
    }
  }
}

void CRigidbody2D::OnCollisionEnter(CRigidbody2D& collider) {
  GetBindObject().OnCollisionEnter(collider);
}

void CRigidbody2D::OnTriggerEnter(CRigidbody2D& collider) {
  GetBindObject().OnTriggerEnter(collider);
}

bool CRigidbody2D::IsTag(const std::string&& tag) const {
  return tag == GetBindObject().GetTagNameOf();
}

bool CRigidbody2D::IsTag(const uint32_t index) const {
  return index == GetBindObject().GetTagIndexOf();
}

} /// ::opgs16::component namespace