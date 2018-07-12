
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
///
/// @file Component/camera_object.cc
///
/// @brief Implementation of Camera component.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-14 Create file and implement basic features.
/// 2018-03-07 Move class implementation into ::opgs16::component.
/// 2018-04-16 Implement functions, SetRear(), SetFar(), and SetFov().
/// 2018-07-01 Refactoring glm::vec3 to DVector3.
///

/// Header file
#include <Component/camera.h>

#include <glm\gtc\matrix_transform.hpp>         /*! glm::ortho */

/// ::opgs16::manager::MSceneManager
#include <Manager/scene_manager.h>
/// import logger
#include <Headers/import_logger.h>

namespace opgs16 {
namespace component {
namespace {

const glm::vec3 y_direction{ 0, 1, 0 };
constexpr float k_ratio{ 256.f / 224.f };

} /*! unnamed namespace */

bool CCamera::s_main_camera_initiated{ false };

CCamera::CCamera(element::CObject& bound_obj, ViewType view_type, CameraType camera_type, bool _auto) :
    CComponent{ bound_obj }, m_viewtype{ view_type }, m_cameratype{ camera_type } {
  SetWorldPosition({ 128, 112, 10 });
  SetLookDirection({ 0, 0, -1 });

  m_view_changed = true; m_proj_changed = true;
  Update(0);

  if (m_cameratype == CameraType::MAIN && !s_main_camera_initiated) {
    s_main_camera_initiated = true;
    manager::scene::GetPresentScene()->SetMainCamera(this);
  }
  else m_cameratype = CameraType::SUB;
}

CCamera::~CCamera() {
  if (m_cameratype == CameraType::MAIN) {
    s_main_camera_initiated = false;
    manager::scene::GetPresentScene()->SetMainCamera(nullptr);
  }
}

void CCamera::SetRear(const float value) {
    if (this->m_viewtype != ViewType::PERSPECTIVE) {
        PUSH_LOG_WARN("You are trying to set rear value into CCamera which is not perspective.");
    }
    else {
        m_near = value;
        m_proj_changed = true;
    }
}

void CCamera::SetFar(const float value) {
    if (this->m_viewtype != ViewType::PERSPECTIVE) {
        PUSH_LOG_WARN("You are trying to set rear value into CCamera which is not perspective.");
    }
    else {
        m_far = value;
        m_proj_changed = true;
    }
}

void CCamera::SetFov(const float value) {
    if (this->m_viewtype != ViewType::PERSPECTIVE) {
        PUSH_LOG_WARN("You are trying to set rear value into CCamera which is not perspective.");
    }
    else {
        if (value < 0.f) {
            PUSH_LOG_WARN("CCamera::SetFov() value is less than 0. Clamped to 0.");
            m_fov = 0.f;
        }
        else if (value > 90.f) {
            PUSH_LOG_WARN("CCamera::SetFov() value is higher than 90. Clamped to 90.");
            m_fov = 90.f;
        }

        m_fov = value;
        m_proj_changed = true;
    }
}

void CCamera::Update(float) {
  if (m_view_changed || m_proj_changed) {
    if (m_view_changed) {
      m_view = glm::lookAt(
          static_cast<glm::vec3>(m_world), static_cast<glm::vec3>(m_world_look),
          y_direction);
      m_view_changed = false;
    }

    if (m_proj_changed) {
      switch (m_viewtype) {
      case ViewType::ORTHO:
        m_projection = glm::ortho(-128.f, 128.f, -112.f, 112.f, m_near, m_far);
        break;
      case ViewType::PERSPECTIVE:
        m_projection = glm::perspective(glm::radians(m_fov), k_ratio, m_near, m_far);
        break;
      }
      m_proj_changed = false;
    }

    m_pv = m_projection * m_view;
  }
}

} /*! opgs16::component */
} /*! opgs16 */