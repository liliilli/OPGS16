#include "canvas.h" /*! Header file */
#include <array>    /*! std::array */
/**
 * camera::CameraObject,
 * camera::CameraObject::ViewType
 */
#include "../../System/Components/Public/camera.h" /*! opgs16::component::CCamera */

namespace canvas {

Canvas::Canvas() : m_is_size_changed{ true } {
    /*! Body */
    using Camera = opgs16::component::CCamera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::SUB);
    m_camera = GetComponent<Camera>();
}

void Canvas::LocalUpdate() {
	if (m_is_size_changed) {
		std::array<GLint, 4> m_viewport_size{0, 0, 256, 224};
		UpdateScreenXYWH(m_viewport_size);
		m_is_size_changed = false;
	}

	UiObject::LocalUpdate();
}

void Canvas::Render() {
    glDisable(GL_DEPTH_TEST);

    for (auto& child : GetChildList())
        child.second->Draw();

    glEnable(GL_DEPTH_TEST);
}

const glm::mat4 Canvas::GetUiCameraPVMatrix() const {
	return m_camera->PvMatrix();
}

}