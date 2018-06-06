
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

#include <Element/object.h>

#include <Manager/physics_manager.h>

#include <Manager/Internal/physics_environment.h>

namespace opgs16::component {

CRigidbody2DCompact::~CRigidbody2DCompact() {
  if (m_motion_state)
    delete m_motion_state;

  if (m_rigidbody != nullptr) {
    if (m_is_collider_bound == ERigidbodyBound::Binded)
      manager::physics::GetManagement().RemoveRigidbody(m_rigidbody);

    delete m_rigidbody;
  }
}

void CRigidbody2DCompact::Update() {

}

std::string CRigidbody2DCompact::GetTemporaryColliderName() {
  return GetBindObject().GetObjectName() + std::to_string(m_value);
}

} /// ::opgs16::component namespace