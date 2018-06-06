#ifndef OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLISION_SHAPE_WRAPPER_H
#define OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLISION_SHAPE_WRAPPER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Internal/physics_collision_shape_wrapper.h
///
/// @brief
/// Wrapper file of bullet library's collison shape class from pre C++11.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-04 Create file.
///

#include <string_view>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

/// ::phitos:: Enhanced assertion.
#include <Phitos/Dbg/assert.h>
/// ::opgs16 Enum types for physics
#include <Helper/Physics/enum_types.h>

namespace opgs16::component::physics {
  class CColliderGeneral;
}

//!
//! Implementation.
//!

namespace opgs16::element::_internal {

class CCollisionShapeObject;

///
/// @class _SharedPtrCount
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-03 Create file.
///
class _SharedPtrCount {
public:
  _SharedPtrCount() = default;

  _SharedPtrCount(const _SharedPtrCount& count) :
      m_pn{count.m_pn} {};

  int32_t Count() const noexcept {
    int32_t count = -1;
    if (m_pn != nullptr)
      count = (*m_pn);

    return count;
  }

  ECollisionShapeUsed IsUsed() const noexcept {
    return m_is_shape_used;
  }

  void Acquire(btCollisionShape* shape_ptr) noexcept {
    if (shape_ptr != nullptr) {
      if (m_pn == nullptr) {
        m_pn = new (std::nothrow) int32_t{0};
        __CheckFailure(m_pn, shape_ptr);
      }
      else {
        ++(*m_pn);
      }
    }
  }

  EShapeReleaseSign Release(btCollisionShape* shape_ptr) noexcept {
    if (m_pn != nullptr) {
        --(*m_pn);
      if (*m_pn == -1) {
        delete shape_ptr;
        delete m_pn;
        m_pn = nullptr;
        return EShapeReleaseSign::Release;
      }
    }
    return EShapeReleaseSign::Wait;
  }

  void __SetUsed() noexcept {
    m_is_shape_used = ECollisionShapeUsed::Used;
  }

private:
  static void __CheckFailure(
      int32_t* const p,
      btCollisionShape* const ptr) noexcept {
    if (p == nullptr) {
      delete ptr;

      PHITOS_UNEXPECTED_BRANCH();
      abort();
    }
  }

  // Reference Counter.
  int32_t* m_pn = nullptr;
  ECollisionShapeUsed m_is_shape_used = ECollisionShapeUsed::NotUsed;
};

///
/// @class CCollisionShapeWrapper
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-03 Create file.
///
/// @todo Implement weak_ptr concept.
///
class CCollisionShapeWrapper {
public:
  CCollisionShapeWrapper() noexcept {}

  CCollisionShapeWrapper(const char* shape_name,
                         EColliderType type,
                         btCollisionShape* shape_ptr) noexcept :
      m_collision_shape_name{shape_name},
      m_type{type} {
    __Acquire(shape_ptr);
  }

  CCollisionShapeWrapper(const CCollisionShapeWrapper& ptr) noexcept :
      m_collision_shape_name{ptr.GetShapeName()},
      m_type{ptr.GetColliderType()},
      m_count{ptr.m_count} {
    PHITOS_ASSERT((ptr.m_shape_ptr != nullptr) || (ptr.m_count->Count() != 0),
        "Unexpected internal malfunction.");
    __Acquire(ptr.GetShape());
  }

  CCollisionShapeWrapper& operator=(const CCollisionShapeWrapper& ptr) noexcept {
    this->m_collision_shape_name = ptr.GetShapeName();
    this->m_type  = ptr.GetColliderType();
    this->m_count = ptr.m_count;
    PHITOS_ASSERT((ptr.m_shape_ptr != nullptr) || (ptr.m_count->Count() != 0),
        "Unexpected internal malfunction.");
    __Acquire(ptr.GetShape());

    return *this;
  }

#ifdef false
  CCollisionShapeWrapper(CCollisionShapeWrapper&&) = delete;
  CCollisionShapeWrapper operator=(CCollisionShapeWrapper&&) = delete;

  CCollisionShapeWrapper(CCollisionShapeWrapper&& ptr) noexcept :
      m_collision_shape_name{ptr.GetShapeName()},
      m_type{ptr.GetColliderType()},
      m_count{ptr.m_count} { }

  CCollisionShapeWrapper& operator=(const CCollisionShapeWrapper& ptr) noexcept {

    return *this;
  }
#endif

  ~CCollisionShapeWrapper() noexcept {
    if (const auto sign = m_count->Release(m_shape_ptr);
        sign == EShapeReleaseSign::Release) {
      delete m_count;
    }
    m_shape_ptr = nullptr;
  }

  inline btCollisionShape* GetShape() const noexcept {
    return m_shape_ptr;
  }

  inline EColliderType GetColliderType() const noexcept {
    return m_type;
  }

  inline const char* GetShapeName() const noexcept {
    return m_collision_shape_name.data();
  }

  inline int32_t GetCount() const noexcept {
    return m_count->Count();
  }

  inline ECollisionShapeUsed IsUsed() const noexcept {
    return m_count->IsUsed();
  }

private:
  void __Acquire(btCollisionShape* shape_ptr) noexcept {
    if (m_count == nullptr) {
      m_count = new _SharedPtrCount();
    }
    m_count->Acquire(shape_ptr);
    m_shape_ptr = shape_ptr;
  }

  void __SetUsed() noexcept {
    PHITOS_ASSERT(m_count != nullptr,
        "The count of CollisionShapeWrapper must not be nullptr.");
    if (m_count == nullptr) {
      return;
    }

    m_count->__SetUsed();
  }

  friend class ::opgs16::component::physics::CColliderGeneral;

  std::string_view  m_collision_shape_name = "";
  EColliderType     m_type = EColliderType::None;
  btCollisionShape* m_shape_ptr = nullptr;
  _SharedPtrCount*  m_count = nullptr;
};

///
/// @brief
/// Make countable collision shape wrapper with shape_name and arguments which
/// are used to create collision shape.
///
/// @param[in] shape_name
/// @param[in] shape_arguments
///
/// @tparam TType
/// @tparam TArgs
///
/// @return
///
template <EColliderType TType, typename... TArgs>
CCollisionShapeWrapper MakeCollisionShapeWrapper(
    const char* shape_name,
    TArgs&&... shape_arguments) noexcept {
  //! Must not be EColliderType::None.
  static_assert(TType != EColliderType::None,
      "EColliderType must not be EColliderType::None.");
  static_assert(
      (TType != EColliderType::Convex2D),
      "Not Implemented.");

  btCollisionShape* shape = nullptr;

  //! compile time branching.
  if constexpr (TType == EColliderType::Box2D) {
    static_assert(sizeof...(TArgs) == 1,
        "The number of EColliderType::Box2D constructor parameter must be 1.");

    shape = new btBox2dShape(std::forward<TArgs>(shape_arguments)...);
  }
  else if constexpr (TType == EColliderType::Box3D) {
    static_assert(sizeof...(TArgs) == 1,
      "The number of EColliderType::Box3D constructor parameter must be 1.");

    shape = new btBoxShape(std::forward<TArgs>(shape_arguments)...);
  }
  else if constexpr (TType == EColliderType::Sphere) {
    static_assert(sizeof...(TArgs) == 1,
      "The number of EColliderType::Sphere constructor parameter must be 1.");

    shape = new btSphereShape(std::forward<TArgs>(shape_arguments)...);
  }

  return {shape_name, TType, shape};
}

} /// ::opgs16::element::_internal namespace

#endif /// OPGS16_ELEMENT_INTERNAL_PHYSICS_COLLISION_SHAPE_WRAPPER_H
