/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file GlobalObjects/Canvas/canvas.cc
 * @brief Definition file of ./canvas.h
 * @log
 * 2018-04-17 Add comments and move definition functions to ::opgs16::element::canvas namespace.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "canvas.h" /*! Header file */
#include <array>    /*! std::array */
/**
 * camera::CameraObject,
 * camera::CameraObject::ViewType
 */
#include "../../System/Components/Public/camera.h" /*! opgs16::component::CCamera */

namespace opgs16 {
namespace element {
namespace canvas {

CCanvas::CCanvas() : m_is_size_changed{ true } {
    /*! Body */
    using Camera = opgs16::component::CCamera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::SUB);
    m_camera = GetComponent<Camera>();
}

void CCanvas::LocalUpdate() {
	if (m_is_size_changed) {
		std::array<GLint, 4> m_viewport_size{0, 0, 256, 224};
		UpdateScreenXYWH(m_viewport_size);
		m_is_size_changed = false;
	}

	UiObject::LocalUpdate();
}

void CCanvas::Render() {
    glDisable(GL_DEPTH_TEST);

    for (auto& child : GetChildList())
        child.second->Draw();

    glEnable(GL_DEPTH_TEST);
}

const glm::mat4 CCanvas::GetUiCameraPVMatrix() const {
	return m_camera->PvMatrix();
}

} /*! opgs16::element::canvas */
} /*! opgs16::element */
} /*! opgs16 */
