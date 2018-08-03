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
#include <Helper/macroes.h>
#include <Manager/physics_manager.h>
#include <Manager/Physics/physics_environment.h>
#include <Phitos/Dbg/assert.h>

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

  // Enroll to unique rigidbody on CObject.
  auto rigidbody_man = GetBindObject().GetComponent<_internal::CRigidbodyBase>();
  if (!rigidbody_man) {
    PHITOS_ASSERT(rigidbody_man, "Rigidbody must be created prior to CCollider.");
  }
  m_bind_rigidbody = rigidbody_man;
  m_bind_rigidbody->pEnrollColliderOnObject(this);
}

CColliderBase::~CColliderBase() {
  using opgs16::manager::physics::RemoveRigidbody;

  // Unbind from binded rigidbody.
  if (m_bind_rigidbody) {
    const auto result = m_bind_rigidbody->pUnbindColliderInContainer(this);
    if (!result) {
      PHITOS_ASSERT(result, "Collider unbinding should have been succeeded.");
    };
  }

  // Release resources
  RemoveRigidbody(m_local_rigidbody);

  if (m_local_rigidbody) {
    delete m_local_rigidbody->getMotionState();
    OP16_SAFE_DELETE(m_local_rigidbody);
  }

  OP16_SAFE_DELETE(m_collision_shape);
}

void CColliderBase::Update(float delta_time) {
  if (m_is_collided_on_this_frame) {
    pfSetBehaviorState(EColliderBehaviorState::Activated);
    m_is_collided_on_this_frame = false;
  }
  else {
    if (m_collision_state != EColliderCollisionState::Idle) {
      // @todo temporary
      pfCallBindObjectCallback(nullptr);
    }
  }
}

btRigidBody** CColliderBase::GetLocalRigidbody() const noexcept {
  return &m_local_rigidbody;
}

btCollisionShape** CColliderBase::GetCollisionShape() const noexcept {
  return &m_collision_shape;
}

uint32_t CColliderBase::GetUniqueIndexValue() const noexcept {
  return m_collider_index;
}

CColliderBase::EColliderActualType CColliderBase::GetColliderType() const noexcept {
  return m_collider_type;
}

const DLinearLimitFactor& CColliderBase::GetLinearFactor() const noexcept {
  return m_linear_factor;
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

void CColliderBase::UpdateLocation() noexcept {
  if (m_local_rigidbody) {
    if (!m_local_rigidbody->isActive())
      m_local_rigidbody->activate();

    btTransform transform = m_local_rigidbody->getCenterOfMassTransform();
    transform.setOrigin(GetBindObject().GetFinalPosition());

    m_local_rigidbody->setWorldTransform(transform);
  }
}

void CColliderBase::ApplyImpulse(const DVector3& vector) {
  if (m_local_rigidbody) {
    if (!m_local_rigidbody->isActive())
      m_local_rigidbody->activate();

    m_local_rigidbody->applyCentralImpulse(vector);
  }
}

  CColliderBase::EColliderBehaviorState CColliderBase::GetBehaviorState() const noexcept {
  return m_behavior_state;
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
  pUpdateColliderTypeFlag();
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

void CColliderBase::pfUpdateAabbToRenderer(const DVector3& min, const DVector3& max) {
  if (m_aabb_renderer) {
    m_aabb_renderer->SetCollisionSize(max - min);
    m_aabb_renderer->SetCollisionRenderPosition((max + min) / 2);
    m_aabb_renderer->Update(0);
    opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
  }
}

void CColliderBase::pfCallBindObjectCallback(CColliderBase* other_collider) {
  const bool is_collision_function =
      !IsTriggered() &&
      (other_collider ? !other_collider->IsTriggered() : true);

  // State machine
  switch (m_collision_state) {
  case EColliderCollisionState::Idle:
    m_collision_state = EColliderCollisionState::Enter;
    break;
  case EColliderCollisionState::Enter:
    if (m_is_collided_on_this_frame)
      m_collision_state = EColliderCollisionState::Stay;
    else
      m_collision_state = EColliderCollisionState::Idle;
    break;
  case EColliderCollisionState::Stay:
    if (m_is_collided_on_this_frame == false)
      m_collision_state = EColliderCollisionState::Idle;
    break;
  }

  GetBindObject().pCallPhysicsCallback(
      m_collision_state,
      is_collision_function, other_collider);
}

bool CColliderBase::pfIsCallbackFunctionCalledOnThisFrame() const noexcept {
  return m_is_collided_on_this_frame;
}

void CColliderBase::pfSetBehaviorState(EColliderBehaviorState state) noexcept {
  PHITOS_ASSERT(state != EColliderBehaviorState::None, "Collision state must not be None.");

  if (state == EColliderBehaviorState::Collided) {
    m_is_collided_on_this_frame = true;
  }

  m_behavior_state = state;
}

void CColliderBase::pUpdateColliderTypeFlag() noexcept {
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
  case EColliderActualType::Static:
    pSetMass(m_mass);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

bool CColliderBase::pInitiateAabbRenderer(bool is_2d) {
  if (m_aabb_renderer) {
    PUSH_LOG_WARN("Aabb Renderer is already created.");
    return false;
  }

  if (is_2d) {
    auto& obj = GetBindObject();
    m_aabb_renderer = std::make_unique<_internal::CPrivateAabbRenderer2D>(obj, this);
    m_aabb_renderer->SetCollisionRenderPosition(obj.GetFinalPosition());
  }
  else {
    PHITOS_NOT_IMPLEMENTED_ASSERT();
  }
  return true;
}

void CColliderBase::pSetColliderUserPointerAndBind() {
  auto& obj = GetBindObject();
  m_bind_info.bind_object   = &obj;
  m_bind_info.bind_collider = this;

  if (!m_local_rigidbody) {
    PHITOS_ASSERT(m_local_rigidbody, "Local btRigidbody must be binded prior to this function.");
  }
  else {
    m_local_rigidbody->setUserPointer(static_cast<void*>(&m_bind_info));
  }
}

} /// ::opgs16::component::_internal namesapce