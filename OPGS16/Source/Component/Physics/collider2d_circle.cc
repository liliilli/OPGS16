#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Physics/collider2d_circle.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-02 Create file.
///

/// Header file
#include <Component/Physics/collider2d_circle.h>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>

#include <Element/object.h>
#include <Manager/physics_manager.h>

namespace {

struct DTempVector3 {
  btScalar x = 0;
  btScalar y = 0;
  btScalar z = 0;

  friend DTempVector3 operator*(const DTempVector3& lhs, float radius) noexcept {
    return DTempVector3{lhs.x * radius, lhs.y * radius, lhs.z * radius};
  }

  friend DTempVector3 operator/(const DTempVector3& lhs, float radius) noexcept {
    return DTempVector3{lhs.x / radius, lhs.y / radius, lhs.z / radius};
  }

  DTempVector3(btScalar x, btScalar y) : x{x}, y{y} {};
  DTempVector3(btScalar x, btScalar y, btScalar z) : x{x}, y{y}, z{z} {};
};

constexpr int32_t small_size  = 8;
constexpr int32_t medium_size = 16;
constexpr int32_t large_size  = 32;
constexpr float   medium_thre = 16.f;
constexpr float   large_thre  = 48.f;

std::array<DTempVector3, small_size> small_unit_circle = {
  DTempVector3{1.f, 0, 0}, {0.7071f, 0.7071f, 0},
  {0, 1.f, 0},  {-0.7071f, 0.7071f, 0},
  {-1.f, 0, 0}, {-0.7071f, -0.7071f, 0},
  {0, -1.f, 0}, {0.7071f, -0.7071f, 0}
};

std::array<DTempVector3, medium_size> medium_unit_circle = {
  DTempVector3{1.f, 0}, {0.9238f, 0.3826f}, {0.7071f, 0.7071f}, {0.3826f, 0.9238f},
  {0, 1.f},  {-0.3826f, 0.9238f}, {-0.7071f, 0.7071f}, {-0.9238f, 0.3826f},
  {-1.f, 0}, {-0.9238f, -0.3826f},{-0.7071f, -0.7071f},{-0.3826f, -0.9238f},
  {0, -1.f}, {0.3826f, -0.9238f}, {0.7071f, -0.7071f}, {0.9238f, -0.3826f},
};

std::array<DTempVector3, large_size> large_unit_circle = {
  DTempVector3{1.f, 0}, {0.9807f, 0.1950f}, {0.9238f, 0.3826f}, {0.8314f, 0.5555f},
  {0.7071f, 0.7071f}, {0.5555f, 0.8314f}, {0.3826f, 0.9238f}, {0.1950f, 0.9807f},

  {0, 1.f},           {-.1950f, 0.9807f}, {-.3826f, 0.9238f}, {-.5555f, 0.8314f},
  {-.7071f, 0.7071f}, {-.8314f, 0.5555f}, {-.9238f, 0.3826f}, {-.9807f, 0.1950f},

  {-1.f, 0},          {-.9807f, -.1950f}, {-.9238f, -.3826f}, {-.8314f, -.5555f},
  {-.7071f, -.7071f}, {-.5555f, -.8314f}, {-.3826f, -.9238f}, {-.1950f, -.9807f},

  {0, -1.f},          {0.1950f, -.9807f}, {0.3826f, -.9238f}, {0.5555f, -.8314f},
  {0.7071f, -.7071f}, {0.8314f, -.5555f}, {0.9238f, -.3826f}, {0.9807f, -.1950f}
};

template <size_type TSize>
std::array<DTempVector3, TSize>
operator*(const std::array<DTempVector3, TSize>& lhs, float r) {
  auto result = lhs;
  for (auto& val : result) {
    val.x *= r;
    val.y *= r;
    val.z *= r;
  }

  return result;
}

} /// unnamed namesapce

namespace opgs16::component {

CColliderCircle2D::CColliderCircle2D(element::CObject& bind_object, float radius) :
    CCollider2DBase{ bind_object } {
  m_collider_radius = radius;
}

void CColliderCircle2D::SetColliderSize(float radius) {
  m_collider_radius = radius;

  if (GetBehaviorState() != EColliderBehaviorState::None) {
    pCreateCollisionShape();
    auto shape = GetCollisionShape();
    auto rigidbody = GetLocalRigidbody();
    (*rigidbody)->setCollisionShape((*shape));
  }
}

void CColliderCircle2D::Update(float delta_time) {
  auto rigidbody = *GetLocalRigidbody();
  if (!rigidbody) return;
  CColliderBase::Update(0);

  // First contact, Reset collider position to CObject's final position.
  if (!m_is_position_initialized) {
    auto motion_state = rigidbody->getMotionState();
    btTransform transform;
    motion_state->getWorldTransform(transform);
    transform.setOrigin(GetBindObject().GetFinalPosition());

    rigidbody->setWorldTransform(transform);
    motion_state->setWorldTransform(transform);
    m_is_position_initialized = true;
  }

  if (!rigidbody->isActive() && GetColliderType() != EColliderActualType::Static) {
    pfSetBehaviorState(EColliderBehaviorState::Sleep);
  }
}

void CColliderCircle2D::pInitializeCollider() {
  auto rigidbody = GetLocalRigidbody();
  pCreatebtRigidbody(pGetMass(), rigidbody);
  pUpdateColliderTypeFlag();

  opgs16::manager::physics::AddRigidbody(*rigidbody);
  pfSetBehaviorState(EColliderBehaviorState::Activated);
}

void CColliderCircle2D::pCreatebtRigidbody(float mass_sum, btRigidBody** rigidbody_ptr) {
  // Create collision shape
  pCreateCollisionShape();

  // Create motion state
  auto& obj = GetBindObject();
  btQuaternion rotation;
  rotation.setEulerZYX(obj.GetFinalRotationAngle(EAxis3D::Z),
                       obj.GetFinalRotationAngle(EAxis3D::Y),
                       obj.GetFinalRotationAngle(EAxis3D::X));
  auto* motionState = new btDefaultMotionState(btTransform{rotation, obj.GetFinalPosition()});

#ifdef false
  btVector3 local_inertia;
  m_collision_shape->calculateLocalInertia(mass_sum, local_inertia);
#endif

  // Create rigidbody info
  auto shape = GetCollisionShape();
  auto body_construction_info = btRigidBody::btRigidBodyConstructionInfo{
      mass_sum, motionState, *shape, btVector3{}};
  body_construction_info.m_restitution = 0.8f;
  body_construction_info.m_friction    = 1.0f;

  // Restrict physical influence to (x, y) axis only.
  *rigidbody_ptr = new btRigidBody{body_construction_info};
  (*rigidbody_ptr)->setLinearFactor({1, 1, 0});
  pSetColliderUserPointerAndBind();
  pInitiateAabbRenderer(true);
}

void CColliderCircle2D::pCreateCollisionShape() {
  auto shape = GetCollisionShape();
  if (*shape) delete *shape;

  if (m_collider_radius < medium_thre) {
    auto datas = small_unit_circle * m_collider_radius;
    (*shape) = new btConvexHullShape{
        reinterpret_cast<btScalar*>(datas.data()),
        small_size, sizeof(DTempVector3)
    };
  }
  else if (m_collider_radius < large_thre) {
    auto datas = medium_unit_circle * m_collider_radius;
    (*shape) = new btConvexHullShape{
        reinterpret_cast<btScalar*>(datas.data()),
        medium_size, sizeof(DTempVector3)
    };
  }
  else {
    auto datas = large_unit_circle * m_collider_radius;
    (*shape) = new btConvexHullShape{
        reinterpret_cast<btScalar*>(datas.data()),
        large_size, sizeof(DTempVector3)
    };
  }

  (*shape)->setMargin(1.f);
}

} /// ::opgs16::component namespace