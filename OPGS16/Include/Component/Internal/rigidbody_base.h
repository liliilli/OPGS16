#ifndef OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H
#define OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/rigidbody_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-01 Create file
///

#include <Component/Internal/collider_base.h>
#include <Component/Internal/component.h>

namespace opgs16::component::_internal {

class CRigidbodyBase : public CComponent {
  using TColliderContainer = std::unordered_map<uint32_t, CColliderBase*>;

public:
  CRigidbodyBase(element::CObject& bind_object) : CComponent{ bind_object } {}

  ///
  /// By releasing, all colliders which are being binded to this instance will
  /// be unbined as rigidbody reference would be set to nullptr.
  ///
  ~CRigidbodyBase();

  CRigidbodyBase(const CRigidbodyBase&) = default;
  CRigidbodyBase(CRigidbodyBase&&)      = default;
  CRigidbodyBase& operator=(const CRigidbodyBase&) = default;
  CRigidbodyBase& operator=(CRigidbodyBase&&)      = default;

protected:
  ///
  /// @brief Find valid collider in container.
  /// This function could be used by checking whether or not it exists.
  ///
  CColliderBase* pFindColliderInContainer(CColliderBase* collider);

  ///
  /// @brief Set collider and bind into this rigidbody instance.
  /// If duplicated, do nothing but just return nullptr.
  ///
  CColliderBase* pSetColliderInContainer(CColliderBase* collider);

  ///
  /// @brief Return collider container binding to this rigidbody instance.
  ///
  TColliderContainer& pGetColliderContainer() noexcept;

  ///
  /// @brief Unbind collider binding into this rigidbody.
  /// If success, return true otherwise false.
  ///
  bool pUnbindColliderInContainer(CColliderBase* collider);

private:
  ///
  /// @brief Enroll collider and forward properties of rigidbody into collider.
  /// Derived rigidbody types have independent implementation
  /// following type's purpose.
  ///
  virtual void pEnrollColliderOnObject(CColliderBase* collider) = 0;

  TColliderContainer m_collider_container;

  friend opgs16::component::_internal::CColliderBase;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRigidbodyBase)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_RIGIDBODY_BASE_H