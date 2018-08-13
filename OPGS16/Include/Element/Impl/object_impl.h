#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Impl/object_impl.h
///
/// @brief Pointer to implementation file of Object.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-19 Refactoring, Inlining, and Removed not-used variables. add class description.
/// 2018-02-19 Add GetParentPosition() method, returns m_summed_world_position.
/// 2018-02-23 Add succeeding flag of translation, rotation, scaling from parent.
/// 2018-03-05 Add rendering layer member functions.
/// 2018-03-11 Moved implementation contents into ::opgs16::element::_internal.
/// 2018-04-14 Change CObjectImpl::SetLocalRotationAngle angle reflection mechanism, restrict bound as (-180.f, 180.f].
/// 2018-04-18 Change function and mechanism of rotation.
/// 2018-07-02 Refactoring.
/// 2018-07-31 Add AddOffset... series function.
///

#include <string>

#include <glm/glm.hpp>
#include <Phitos/Enums/activated.h>

/// ::opgs16::element::EAxis3D
#include <Helper/Math/math.h>
#include <Helper/Type/axis.h>
#include <Helper/Type/vector3.h>
#include "Helper/Type/quaternion.h"

namespace opgs16::element::_internal {
///
/// @class CObjectImpl
/// @brief Pointer to implementation of class Object.
///
class CObjectImpl final {
public:
  //!
  //! Position
  //!

  inline const DVector3& GetIndependentLocalPosition() const {
    return m_independent_local_position;
  }

  inline const DVector3& GetIndependentWorldPosition() const {
    return m_independent_world_position;
  }

  inline const DVector3& GetAxisAlignedSummedWorldPosition() const {
    if (m_is_summed_position_dirty) pUpdateAxisAlignedSummedWorldPosition();
    return m_summed_world_position;
  }

  inline const DVector3& GetFinalRenderingPosition() const {
    if (m_is_final_rotation_angle_dirty) {
      pUpdateFinalWorldRotationEulerAngle();
    }
    if (m_is_final_position_dirty) {
      pUpdateAxisAlignedFinalPosition();
    }

    return m_axis_aligned_final_position;
  }

  inline void SetLocalPosition(const DVector3& position) noexcept {
    m_independent_local_position = position;

    m_is_model_matrix_dirty   = true;
    m_is_local_position_dirty = true;
    m_is_final_position_dirty = true;
  }

  inline void SetWorldPosition(const DVector3& position) noexcept {
    m_independent_world_position     = position;

    m_is_model_matrix_dirty   = true;
    m_is_world_position_dirty = true;
    m_is_final_position_dirty = true;
    m_is_summed_position_dirty  = true;
  }

  inline void SetWorldPosWithFinalPos(const DVector3& final_position) noexcept {
    SetWorldPosition(final_position - m_propagated_world_basis_position);
  }

  inline void SetWorldPropagatedPosition(const DVector3& propagated_world_basis_position) noexcept {
    m_propagated_world_basis_position = propagated_world_basis_position;
    SetWorldPosition(m_independent_world_position);
  }

  ///
  /// @brief Add offset value with axis as local position.
  ///
  void AddOffsetLocalPosition(EAxis3D axis, float value) noexcept {
    switch (axis) {
    case EAxis3D::X: m_independent_local_position.x += value; break;
    case EAxis3D::Y: m_independent_local_position.y += value; break;
    case EAxis3D::Z: m_independent_local_position.z += value; break;
    }

    m_is_model_matrix_dirty   = true;
    m_is_local_position_dirty = true;
    m_is_final_position_dirty = true;
  }

  ///
  /// @brief Add offset value with axis as world position.
  ///
  void AddOffsetWorldPosition(EAxis3D axis, float value) noexcept {
    switch (axis) {
    case EAxis3D::X: m_independent_world_position.x += value; break;
    case EAxis3D::Y: m_independent_world_position.y += value; break;
    case EAxis3D::Z: m_independent_world_position.z += value; break;
    }

    m_summed_world_position   = m_propagated_world_basis_position + m_independent_world_position;
    m_is_model_matrix_dirty   = true;
    m_is_world_position_dirty = true;
    m_is_final_position_dirty = true;
    m_is_summed_position_dirty  = true;
  }

  //!
  //! Rotation
  //!

  const std::array<DVector3, 3>& GetChildObjectWorldAxisBasisValue() noexcept {
    if (m_is_world_propagation_axis_dirty) pUpdateObjectSummedAxisBasis();
    return m_object_propagate_axis;
  }

  const std::array<DVector3, 3>& GetObjectWorldAxisBasisValue() const noexcept {
    if (m_is_world_space_axis_dirty) pUpdateObjectSpaceAxisBasis();

    return m_object_space_axis;
  }

  void SetObjectWorldAxisBasisValue(const std::array<DVector3, 3>& propagated_axis) noexcept {
    m_object_space_axis = propagated_axis;

    m_is_model_matrix_dirty           = true;
    m_is_final_position_dirty         = true;
    m_is_summed_rotation_angle_dirty  = true;
    m_is_final_rotation_angle_dirty   = true;
    m_is_world_propagation_axis_dirty = true;
  }

  void SetObjectWorldRotationBasisValue(const DVector3& propagated_world_rotation_angle) noexcept {
    m_propagated_world_rotation_euler_angle = propagated_world_rotation_angle;

    m_is_model_matrix_dirty           = true;
    m_is_final_position_dirty         = true;
    m_is_summed_rotation_angle_dirty  = true;
    m_is_final_rotation_angle_dirty   = true;
    m_is_world_propagation_axis_dirty = true;
  }

  const DVector3& GetLocalRotationEulerAngle() const noexcept {
    return m_local_rotation_euler_angle;
  }

  const DVector3& GetWorldRotationEulerAngle() const noexcept {
    return m_world_rotation_euler_angle;
  }

  float GetLocalRotationAngle(const EAxis3D direction) const noexcept {
    switch (direction) {
    case EAxis3D::X: return m_local_rotation_euler_angle.x;
    case EAxis3D::Y: return m_local_rotation_euler_angle.y;
    case EAxis3D::Z: return m_local_rotation_euler_angle.z;
    }
    return 0;
  }

  float GetWorldRotationAngle(const EAxis3D direction) const noexcept {
    switch (direction) {
    case EAxis3D::X: return m_world_rotation_euler_angle.x;
    case EAxis3D::Y: return m_world_rotation_euler_angle.y;
    case EAxis3D::Z: return m_world_rotation_euler_angle.z;
    }
    return 0;
  }

  float GetFinalRotationAngle(const EAxis3D direction) const {
    switch (direction) {
    case EAxis3D::X: return m_object_final_rotation_euler_angle.x;
    case EAxis3D::Y: return m_object_final_rotation_euler_angle.y;
    case EAxis3D::Z: return m_object_final_rotation_euler_angle.z;
    }
    return 0;
  }

  const DVector3& GetWorldSummedRotationAngle() const noexcept {
    if (m_is_summed_rotation_angle_dirty) pUpdateSummedWorldRotationEulerAngle();
    return m_summed_world_rotation_euler_angle;
  }

  void SetLocalRotationAngle(const EAxis3D direction, const float angle_value) noexcept {
    const auto angle = math::GetRotationAngle(angle_value);

    switch (direction) {
    case EAxis3D::X: m_local_rotation_euler_angle.x = angle; break;
    case EAxis3D::Y: m_local_rotation_euler_angle.y = angle; break;
    case EAxis3D::Z: m_local_rotation_euler_angle.z = angle; break;
    default: break;
    }

    m_is_model_matrix_dirty           = true;
    m_is_local_rotation_angle_dirty   = true;
    m_is_final_rotation_angle_dirty   = true;
    m_local_rotation_quaternion = DQuaternion(m_local_rotation_euler_angle);
  }

  void SetWorldRotationAngle(const EAxis3D direction, const float angle_value) noexcept {
    const auto angle = math::GetRotationAngle(angle_value);

    switch (direction) {
    case EAxis3D::X: m_world_rotation_euler_angle.x = angle; break;
    case EAxis3D::Y: m_world_rotation_euler_angle.y = angle; break;
    case EAxis3D::Z: m_world_rotation_euler_angle.z = angle; break;
    default: break;
    }

    m_is_model_matrix_dirty           = true;
    m_is_summed_rotation_angle_dirty  = true;
    m_is_world_space_axis_dirty       = true;
    m_is_world_propagation_axis_dirty = true;
    m_is_final_rotation_angle_dirty   = true;

    //pUpdateSummedWorldRotationEulerAngle();
  }

  void AddOffsetLocalAngle(EAxis3D axis, const float angle_value) noexcept {
    switch (axis) {
    case EAxis3D::X: m_local_rotation_euler_angle.x += angle_value; break;
    case EAxis3D::Y: m_local_rotation_euler_angle.y += angle_value; break;
    case EAxis3D::Z: m_local_rotation_euler_angle.z += angle_value; break;
    default: break;
    }

    m_is_model_matrix_dirty           = true;
    m_is_local_rotation_angle_dirty   = true;
    m_is_final_rotation_angle_dirty   = true;
  }

  void AddOffsetWorldAngle(EAxis3D axis, const float angle_value) noexcept {
    switch (axis) {
    case EAxis3D::X: m_world_rotation_euler_angle.x += angle_value; break;
    case EAxis3D::Y: m_world_rotation_euler_angle.y += angle_value; break;
    case EAxis3D::Z: m_world_rotation_euler_angle.z += angle_value; break;
    default: break;
    }

    m_is_model_matrix_dirty           = true;
    m_is_summed_rotation_angle_dirty  = true;
    m_is_world_space_axis_dirty       = true;
    m_is_world_propagation_axis_dirty = true;
    m_is_final_rotation_angle_dirty   = true;
  }

  //!
  //! Scale
  //!

  const DVector3& GetLocalScale() const noexcept {
    return m_local_scale;
  }

  inline void SetLocalScale(const DVector3& local_scale) noexcept {
    m_local_scale = local_scale;

    m_is_model_matrix_dirty = true;
    m_is_local_scale_dirty = true;
  }

  inline void SetScaleParentFactor(const DVector3& scale_factor) noexcept {
    m_propagated_producted_scale = scale_factor;
  }

  //!
  //! Matrix
  //!

  const glm::mat4& GetModelMatrix() const;

  ///
  /// @brief Set active option of object.
  /// If m_active is false, this object cannot update until m_active return to true.
  /// @param[in] value Active option value.
  ///
  inline void SetActive(const bool value) {
    m_is_object_activated = value ? EActivated::Activated : EActivated::Disabled;
  }

  inline phitos::enums::EActivated IsActive() const {
    return m_is_object_activated;
  }

  inline void CalculateActivation() noexcept {
    const auto previous = m_is_finally_activated;

    if (m_is_any_parent_activated == EActivated::Disabled ||
        m_is_object_activated == EActivated::Disabled) {
      m_is_finally_activated = EActivated::Disabled;
    }
    else {
      m_is_finally_activated = EActivated::Activated;
    }

    if (previous != m_is_finally_activated) {
      m_is_callback_called = false;
    }
  }

  inline void PropagateActivation(const phitos::enums::EActivated value) {
    m_is_any_parent_activated = value;
  }

  inline phitos::enums::EActivated IsAnyParentActivated() const noexcept {
    return m_is_any_parent_activated;
  }

  inline phitos::enums::EActivated IsFinallyActivated() const {
    return m_is_finally_activated;
  }

  inline bool IsCallbackNotCalled() const noexcept {
    return !m_is_callback_called;
  }

  inline void SetCallbackFlagToFalse() noexcept {
    m_is_callback_called = true;
  }

  ///
  /// @brief Set tag with tag name. This method will check whether or not exist matched tag name
  /// in SettingManager. If not exist, do nothing and chagne error flag.
  /// @param[in] tag_name Tag name
  ///
  void SetTag(const std::string& tag_name);

  ///
  /// @brief Overloaded version of SetObjectTag(tag_name)
  ///
  void SetTag(const unsigned tag_index);

  ///
  /// @brief Return tag index value of this object.
  ///
  inline unsigned GetTagIndexOf() const noexcept {
    return m_tag_index;
  }

  ///
  /// @brief Get Tag Name of this object. (different with name of object)
  /// This methods return tag name by referencing SettingManager in body.
  /// @return Tag name string.
  ///
  std::string GetTagNameOf() const;

private:
  ///
  /// @brief Update propagation axis basis using m_summed_world_rotation_euler_angle.
  ///
  void pUpdateObjectSummedAxisBasis() noexcept;

  ///
  /// @brief Update propagation axis basis using m_propagated_world_rotation_euler_angle.
  ///
  void pUpdateObjectSpaceAxisBasis() const noexcept;

  ///
  /// @brief
  ///
  void pUpdateLocalRotationQuaternion() const noexcept;

  ///
  /// @brief Update summed world rotation euler angle using parent_summed_world + this_world.
  ///
  void pUpdateSummedWorldRotationEulerAngle() const noexcept;

  ///
  /// @brief
  ///
  void pUpdateAxisAlignedLocalPosition() const noexcept;

  ///
  /// @brief
  ///
  void pUpdateAxisAlignedWorldPosition() const noexcept;

  ///
  /// @brief
  ///
  void pUpdateAxisAlignedSummedWorldPosition() const noexcept;

  ///
  /// @brief
  ///
  void pUpdateFinalWorldRotationEulerAngle() const noexcept;

  ///
  /// @brief Refresh Translation matrix.
  ///
  void pUpdateAxisAlignedFinalPosition() const;

  void RefreshScaleVector() const;	/** Refresh Scaling matrix */

  /// (x, y, z) local position.
  DVector3 m_independent_local_position;
  /// (x, y, z) world position.
  DVector3 m_independent_world_position;
  /// (x, y, z) final position of parent
  DVector3 m_propagated_world_basis_position;

  /// m_object_space_axis * local_position;
  mutable DVector3 m_local_axis_arranged_position;
  /// m_object_space_axis * world_position;
  mutable DVector3 m_world_axis_arranged_position;
  /// (x, y, z) parent position to bring child.
  /// m_propagated_world_basis_position + m_world_axis_arranged_position.
  mutable DVector3 m_summed_world_position;
  /// (x, y, z) final position in hierarchy.
  /// m_summed_world_position + m_local_axis_arranged_position
  mutable DVector3 m_axis_aligned_final_position;

  DVector3 m_local_rotation_euler_angle;
  DVector3 m_world_rotation_euler_angle;
  /// parent's propagated world rotation summed angle;
  DVector3 m_propagated_world_rotation_euler_angle;
  /// propagated_world_rotation_euler_angle + world
  mutable DVector3 m_summed_world_rotation_euler_angle;
  /// local + summed_world
  mutable DVector3 m_object_final_rotation_euler_angle;

  mutable DQuaternion m_local_rotation_quaternion;
  mutable glm::mat3   m_local_rotation_matrix;

  mutable DQuaternion m_propagated_rotation_quaternion;
  mutable glm::mat3   m_propagated_rotation_matrix;

  mutable DQuaternion m_summed_rotation_quaternion;
  mutable DQuaternion m_final_rotation_quaternion;

  /// Scale local factor, default is (1, 1, 1)
  DVector3 m_local_scale = DVector3{ 1.f };
  /// Scale world factor. default is (1, 1, 1)
  DVector3 m_world_scale = DVector3{ 1.f };
  /// Scale factor from parent.
  DVector3 m_propagated_producted_scale = DVector3{ 1.f };
  /// m_world_scale * m_propagated_producted_scale
  DVector3 m_summed_producted_scale = DVector3{ 1.f };
  /// m_summed_producted_scale * m_local_scale
  mutable DVector3 m_rendering_producted_scale = DVector3{ 1.f };
  /// (x, y, z) scale vector to apply to matrix.
  /// @todo deprecated
  mutable DVector3 m_scale_final_vector;

  /// World + Parent rotation matrix.
  mutable glm::mat3 m_summed_rotation_matrix;
  /// Final model matrix also reflected by parent's and world rot.
  mutable glm::mat4 m_final_model;

  //!
  //! Axis variables.
  //!

  /// Used as this object coordinate space.
  mutable std::array<DVector3, 3> m_object_space_axis = {
    DVector3::RightX(), DVector3::UpY(), DVector3::FrontZ()
  };
  /// Used as child object's coordinate space.
  mutable std::array<DVector3, 3> m_object_propagate_axis = {
    DVector3::RightX(), DVector3::UpY(), DVector3::FrontZ()
  };

  using EActivated = phitos::enums::EActivated;
  /// Object all update activation variable.
  EActivated m_is_object_activated      = EActivated::Activated;
  EActivated m_is_any_parent_activated  = EActivated::Activated;
  /// Any parent all update activation variable.
  EActivated m_is_finally_activated     = EActivated::Activated;

  bool m_is_callback_called = false;

  //!
  //! Flags
  //!

  mutable bool m_is_world_propagation_axis_dirty= true;
  mutable bool m_is_world_space_axis_dirty      = true;

  mutable bool m_is_local_rotation_angle_dirty  = true;
  mutable bool m_is_summed_rotation_angle_dirty = true;
  mutable bool m_is_final_rotation_angle_dirty  = true;

  mutable bool m_is_local_scale_dirty = true;

  mutable bool m_is_local_position_dirty = true;
  mutable bool m_is_world_position_dirty = true;
  mutable bool m_is_summed_position_dirty = true;
  mutable bool m_is_final_position_dirty = true;

  mutable bool m_is_model_matrix_dirty = true;

  unsigned m_tag_index = 0;
};

} /// ::opgs16::element::_internal namespace

#endif /** OPGS16_SYSTEM_OBJECT_OBJECT_PRIVATE_IMPLEMENTATION_H */