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

/*!
 * @file System/Components/Private/camera_object.cc
 * @brief Implementation of Camera component.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and implement basic features.
 * 2018-03-07 Move class implementation into ::opgs16::component.
 */

#include "../Public/camera.h"   /*! Header file */

#include <glm/gtc/matrix_transform.hpp>         /*! glm::ortho */
#include "../../Manager/Public/scene_manager.h" /*! MSceneManager */

namespace opgs16 {
namespace component {
namespace {
using Object = element::CObject;
const glm::vec3 y_direction{ 0, 1, 0 };

/*! Initiate orthographic projection. automatically set it to screen size. */
void InitiateOrthographicProjection(glm::mat4& view, glm::mat4& projection, glm::mat4& pv) {
    view = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, y_direction);
    projection = glm::ortho<float>(0.f, 256.f, 0.f, 224.f);
    pv = projection * view;
}

/*! Initiate perspective projection matrix. automatically set it to screen size ratio. */
void InitiatePerspectiveProjection(glm::mat4& view, glm::mat4& projection, glm::mat4& pv) {
    view = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, y_direction);

	const auto fovy = glm::radians(50.f);
	const auto ratio = (256.f / 224.f);
	projection = glm::perspective(fovy, ratio, 0.03f, 100.f);
    pv = projection * view;
}
} /*! unnamed namespace */

bool CCamera::s_main_camera_initiated{ false };

CCamera::CCamera(Object& bound_obj, ViewType view_type, CameraType camera_type, bool _auto) :
    CComponent{ bound_obj }, m_viewtype{ view_type }, m_cameratype{ camera_type } {
    /*! Body */
	switch (m_viewtype) {
	case ViewType::ORTHO:       InitiateOrthographicProjection(m_view, m_projection, m_PV); break;
    case ViewType::PERSPECTIVE: InitiatePerspectiveProjection(m_view, m_projection, m_PV);  break;
	}

    if (m_cameratype == CameraType::MAIN && !s_main_camera_initiated) {
        s_main_camera_initiated = true;
        manager::MSceneManager::Instance().PresentScene()->SetMainCamera(this);
    }
    else
        m_cameratype = CameraType::SUB;
}

CCamera::~CCamera() {
    if (m_cameratype == CameraType::MAIN) {
        s_main_camera_initiated = false;
        manager::MSceneManager::Instance().PresentScene()->SetMainCamera(nullptr);
    }
}

const glm::mat4& CCamera::ViewMatrix() const noexcept {
    return m_view;
}

const glm::mat4& CCamera::ProjectionMatrix() const noexcept {
    return m_projection;
}

const glm::mat4& CCamera::PvMatrix() const noexcept {
    return m_PV;
}

void CCamera::Update() {
    if (m_information_changed) {
        m_view = glm::lookAt(m_world, m_world_look, y_direction);
        m_information_changed = false;
    }
}

} /*! opgs16::component */
} /*! opgs16 */