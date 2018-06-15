/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Components/Private/camera_object.cc
 * @brief Implementation of Camera component.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and implement basic features.
 * 2018-03-07 Move class implementation into ::opgs16::component.
 * 2018-04-16 Implement functions, SetRear(), SetFar(), and SetFov().
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Component\camera.h>       /// Header file

#include <glm\gtc\matrix_transform.hpp>         /*! glm::ortho */

#include <Manager\scene_manager.h>  /// ::opgs16::manager::MSceneManager
#include <Headers\import_logger.h>  /// import logger

namespace opgs16 {
namespace component {
namespace {

const glm::vec3 y_direction{ 0, 1, 0 };
constexpr float k_ratio{ 256.f / 224.f };

} /*! unnamed namespace */

bool CCamera::s_main_camera_initiated{ false };

CCamera::CCamera(element::CObject& bound_obj, ViewType view_type, CameraType camera_type, bool _auto) :
    CComponent{ bound_obj }, m_viewtype{ view_type }, m_cameratype{ camera_type } {
    /*! Body */
    SetWorldPosition({ 128, 112, 0 });
    SetLookDirection({ 0, 0, -1 });

	switch (m_viewtype) {
	case ViewType::ORTHO:
        m_fov = m_near  = m_far = 0.f;
        break;
    case ViewType::PERSPECTIVE:
        m_fov = 50.f; m_near = 0.1f; m_far = 100.f;
        break;
	}
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
            m_view = glm::lookAt(m_world, m_world_look, y_direction);
            m_view_changed = false;
        }
        if (m_proj_changed) {
            switch (m_viewtype) {
            case ViewType::ORTHO:
                m_projection = glm::ortho<float>(-128.f, 128.f, -112.f, 112.f);
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