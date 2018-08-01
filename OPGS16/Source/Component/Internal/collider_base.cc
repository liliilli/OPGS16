#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/collider_base.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file.
///

/// Header file
#include <Component/Internal/collider_base.h>

#include <limits>

#include <Component/Internal/rigidbody_base.h>
#include <Element/object.h>
#include <Manager/physics_manager.h>
#include <Manager/Physics/physics_environment.h>
#include <Phitos/Dbg/assert.h>

#define OP16_SAFE_DELETE(__MAInstance__) \
  delete __MAInstance__; \
  __MAInstance__ = nullptr

namespace {

uint32_t GetColliderUniqueIndexValue() {
  static uint32_t s_collider_index_number = 0;

  if (s_collider_index_number == std::numeric_limits<uint32_t>::max())
    s_collider_index_number = 0;
  else
    ++s_collider_index_number;

  return s_collider_index_number;
}

} /// ::unnamed namespace

namespace opgs16::component::_internal {

CColliderBase::CColliderBase(element::CObject& bind_object) :
    CComponent{bind_object} {
  m_collider_index = GetColliderUniqueIndexValue();

  auto& obj = GetBindObject();

  // Enroll to unique rigidbody on CObject.
  auto rigidbody_man = obj.GetComponent<_internal::CRigidbodyBase>();
  if (!rigidbody_man) {
    PHITOS_ASSERT(rigidbody_man, "Rigidbody must be created prior to CCollider.");
  }
  m_bind_rigidbody = rigidbody_man;
  m_bind_rigidbody->pEnrollColliderOnObject(this);
}

CColliderBase::~CColliderBase() {
  // Unbind from binded rigidbody.
  if (m_bind_rigidbody) {
    const auto result = m_bind_rigidbody->pUnbindColliderInContainer(this);
    if (!result) {
      PHITOS_ASSERT(result, "Collider unbinding should have been succeeded.");
    };
  }

  // Release resources
  using opgs16::manager::physics::RemoveRigidbody;
  RemoveRigidbody(m_local_rigidbody);

  if (m_local_rigidbody) {
    delete m_local_rigidbody->getMotionState();
    OP16_SAFE_DELETE(m_local_rigidbody);
  }

  OP16_SAFE_DELETE(m_collision_shape);
}

uint32_t CColliderBase::GetUniqueIndexValue() const noexcept {
  return m_collider_index;
}

CColliderBase::EColliderActualType
CColliderBase::GetColliderType() const noexcept {
  return m_collider_type;
}

const DLinearLimitFactor& CColliderBase::GetLinearFactor() const noexcept {
  return m_linear_factor;
}

btRigidBody** CColliderBase::GetLocalRigidbody() const noexcept {
  return &m_local_rigidbody;
}

btCollisionShape** CColliderBase::GetCollisionShape() const noexcept {
  return &m_collision_shape;
}

void CColliderBase::SetTriggered(bool is_triggered) {
  auto rigidbody = *GetLocalRigidbody();
  if (!rigidbody) {
    PUSH_LOG_ERROR_EXT("{}'s btRigidbody is not set up.", GetUniqueIndexValue());
    return;
  }

  if (is_triggered) {
    rigidbody->setCollisionFlags(
        rigidbody->getCollisionFlags() |
        btCollisionObject::CF_NO_CONTACT_RESPONSE);
  }
  else if (rigidbody->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
    rigidbody->setCollisionFlags(
        rigidbody->getCollisionFlags() &
        ~btCollisionObject::CF_NO_CONTACT_RESPONSE
    );
  }

  m_is_collision_triggered = is_triggered;
}

bool CColliderBase::IsTriggered() const noexcept {
  return m_is_collision_triggered;
}


float CColliderBase::pGetMass() const noexcept {
  return m_mass;
}

void CColliderBase::pSetMass(float mass_value) noexcept {
  m_mass = mass_value;
  if (m_mass < 0.001f)
    m_mass = 0.001f;

  if (m_collider_type != EColliderActualType::Dynamic)
    m_actual_mass = 0;
  else
    m_actual_mass = m_mass;

  if (m_local_rigidbody) {
    m_local_rigidbody->setMassProps(m_actual_mass, {0, 0, 0});
  }
}

void CColliderBase::pSetColliderType(EColliderActualType collider_type) noexcept {
  PHITOS_ASSERT(collider_type != EColliderActualType::None,
                "None as collider type is prohibited.");

  m_collider_type = collider_type;

  if (!m_local_rigidbody) return;
  __pUpdateFlags();
}

void CColliderBase::pSetLinearFactor(const DLinearLimitFactor& linear_factor) noexcept {
  m_linear_factor = linear_factor;

  if (!m_local_rigidbody) return;
  m_local_rigidbody->setLinearFactor(btVector3{
      m_linear_factor.x ? 1.f : 0.f,
      m_linear_factor.y ? 1.f : 0.f,
      m_linear_factor.z ? 1.f : 0.f});
}

void CColliderBase::pSetUsingGravity(bool use_gravity) noexcept {
  m_is_use_gravity = use_gravity;

  if (!m_local_rigidbody) return;
  if (m_is_use_gravity) {
    using opgs16::manager::physics::GetManagement;
    const auto& gravity = GetManagement()->GetGlobalGravity();
    m_local_rigidbody->setGravity(gravity);
  }
  else {
    m_local_rigidbody->setGravity({0, 0, 0});
  }
}

void CColliderBase::__pUpdateFlags() noexcept {
  switch (m_collider_type) {
  case EColliderActualType::Kinetic:
    pSetMass(m_mass);
    m_local_rigidbody->setCollisionFlags(m_local_rigidbody->getCollisionFlags() |
                                         btCollisionObject::CF_KINEMATIC_OBJECT);
    break;
  case EColliderActualType::Dynamic:
    pSetMass(m_mass);
    m_local_rigidbody->setCollisionFlags(m_local_rigidbody->getCollisionFlags() &
                                         ~btCollisionObject::CF_KINEMATIC_OBJECT);
    break;
  case EColliderActualType::Staic:
    pSetMass(m_mass);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::opgs16::component::_internal namesapce