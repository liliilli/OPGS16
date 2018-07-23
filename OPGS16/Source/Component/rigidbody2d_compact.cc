
///
/// @file Component/rigidbody2d_compact.cc
///
/// @brief
///
///
/// @author Jongmin Yun
/// @log
/// 2018-06-06 Create file.
///

/// Header file
#include <Component/rigidbody2d_compact.h>

#include <Core/application.h>
#include <Core/core_setting.h>

#include <Element/object.h>

#include <Helper/aabb_infobox.h>

#include <Manager/object_manager.h>

#include <Manager/physics_manager.h>

#include <Manager/Internal/physics_environment.h>

namespace opgs16::component {

CRigidbody2DCompact::~CRigidbody2DCompact() {
  if (m_motion_state)
    delete m_motion_state;

  if (m_rigidbody != nullptr) {
    if (m_is_collider_bound == ERigidbodyBound::Binded) {
      manager::physics::GetManagement().RemoveRigidbody(m_rigidbody);

      auto& shape_list = manager::physics::GetShapeList();
      shape_list.RemoveCollisionShape(m_collider_hash_name.c_str());
    }

    delete m_rigidbody;
  }
}

void CRigidbody2DCompact::Update(float) {
  using opgs16::setting::IsEnableRenderingAabb;
  using opgs16::manager::object::InsertAABBInformation;

  if (m_rigidbody) {
    btVector3 _min, _max;
    m_rigidbody->getAabb(_min, _max);

#ifdef false
    PUSH_LOG_INFO_EXT("Test AABB Min position : {} {} {}",
                      _min.getX(), _min.getY(), _min.getZ());
    PUSH_LOG_INFO_EXT("Test AABB Max position : {} {} {}",
                      _max.getX(), _max.getY(), _max.getZ());
#endif
  }

  if (m_rigidbody && IsEnableRenderingAabb()) {
    btVector3 _min, _max;
    m_rigidbody->getAabb(_min, _max);

    InsertAABBInformation(EAABBStyle::_2D, DAABBInfoBox(_min, _max));
  }
}

std::string CRigidbody2DCompact::GetTemporaryColliderName() {
  return GetBindObject().GetGameObjectName() + std::to_string(m_value);
}

void CRigidbody2DCompact::UpdateTransform() {
  auto& object = GetBindObject();

  const auto& final_position = object.GetFinalPosition();
  auto x_rot_deg = object.GetRotationWpAngle(element::_internal::EDirection::X);
  auto y_rot_deg = object.GetRotationWpAngle(element::_internal::EDirection::Y);
  auto z_rot_deg = object.GetRotationWpAngle(element::_internal::EDirection::Z);

  //!
  //! @todo NEED TO REVISION.
  //!

  switch (m_object_physics_type) {
  case EDynamic::Dynamic:
    m_transform.setOrigin({final_position.x, final_position.y, final_position.z});
    if (m_motion_state && m_is_collider_bound == ERigidbodyBound::Binded)
      m_motion_state->setWorldTransform(m_transform);
    break;
  case EDynamic::Kinetic:
    m_transform.setOrigin({final_position.x, final_position.y, final_position.z});
    if (m_motion_state && m_is_collider_bound == ERigidbodyBound::Binded)
      m_motion_state->setWorldTransform(m_transform);
    break;
  case EDynamic::Static:
    m_transform.setOrigin({final_position.x, final_position.y, final_position.z});
    if (m_motion_state && m_is_collider_bound == ERigidbodyBound::Binded)
      m_motion_state->setWorldTransform(m_transform);
    break;
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::opgs16::component namespace