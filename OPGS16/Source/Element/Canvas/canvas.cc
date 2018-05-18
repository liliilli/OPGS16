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
 * 2018-04-17 Move file path to System/Element/Canvas/Private.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\Canvas\canvas.h>  /// Header file

#include <array>                    /// std::array

#include <Component\camera.h>       /// ::opgs16::component::CCamera

namespace {
constexpr std::array<GLint, 4> k_viewport_size{0, 0, 256, 224};
} /*! unnamed namespace */

namespace opgs16::element::canvas {

CCanvas::CCanvas() : m_is_size_changed{ true } {
    using Camera = component::CCamera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::SUB);
    m_camera = GetComponent<Camera>();
}

void CCanvas::LocalUpdate() {
	if (m_is_size_changed) {
		UpdateScreenXYWH(k_viewport_size);
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
