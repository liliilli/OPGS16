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

#include <array>                /*! std::array<GLint, 4> */
#include <GL/glew.h>            /*! type specifier */
#include <glm/gtc/matrix_transform.hpp>         /*! glm::ortho */
#include "../../Manager/Public/scene_manager.h" /*! SceneManager */

namespace opgs16 {
namespace component {
namespace {
using Object = element::Object;
} /*! unnamed namespace */

bool Camera::s_main_camera_initiated{ false };

Camera::Camera(Object& bound_obj, ViewType view_type, CameraType camera_type, bool _auto) :
    _internal::Component{ bound_obj }, m_viewtype{ view_type }, m_cameratype{ camera_type } {
    /*! Body */
	switch (m_viewtype) {
	case ViewType::ORTHO:       InitiateOrthographicProjection();   break;
    case ViewType::PERSPECTIVE: InitiatePerspectiveProjection();    break;
	}

    if (m_cameratype == CameraType::MAIN && !s_main_camera_initiated) {
        /*!
         * If the scene have not main camera, signal scene to bind it to scene
         * to display world.
         */
        s_main_camera_initiated = true;
        opgs16::manager::SceneManager::Instance().PresentScene()->SetMainCamera(this);
    }
    else
        m_cameratype = CameraType::SUB;
}

Camera::~Camera() {
    if (m_cameratype == CameraType::MAIN) {
        s_main_camera_initiated = false;
        opgs16::manager::SceneManager::Instance().PresentScene()->SetMainCamera(nullptr);
    }
}

const glm::mat4 Camera::GetViewMatrix() const noexcept {
    return m_view;
}

const glm::mat4 Camera::GetProjectionMatrix() const noexcept {
    return m_projection;
}

const glm::mat4 Camera::GetPV() const noexcept {
    return m_PV;
}

void Camera::InitiateOrthographicProjection() {
    std::array<GLint, 4> viewport_size{0, 0, 256, 224};
	//glGetIntegerv(GL_VIEWPORT, &viewport_size[0]);

    // 이동은 구현하지 않았음.
    m_view = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
	m_projection = glm::ortho<float>(static_cast<float>(viewport_size[0]),
                                     static_cast<float>(viewport_size[2]),
                                     static_cast<float>(viewport_size[1]),
                                     static_cast<float>(viewport_size[3]));
    m_PV = m_projection * m_view;
}

void Camera::InitiatePerspectiveProjection() {
    std::array<GLint, 4> viewport_size{0, 0, 256, 224};
	//glGetIntegerv(GL_VIEWPORT, &viewport_size[0]);

    m_view = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

	auto fovy = glm::radians(50.f);
	auto ratio = static_cast<float>(viewport_size[2]) / viewport_size[3];
	m_projection = glm::perspective(fovy, ratio, 0.03f, 100.f);
    m_PV = m_projection * m_view;
}

void Camera::Update() {
}

} /*! opgs16::component */
} /*! opgs16 */