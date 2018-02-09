#include "canvas.h"

/** std::array */
#include <array>
/**
 * camera::CameraObject,
 * camera::CameraObject::ViewType
 */
#include "..\..\GlobalObjects\camera_object.h"

namespace Canvas {
Canvas::Canvas() :
	m_camera{ std::make_unique<camera::CameraObject>(camera::CameraObject::ViewType::ORTHO,
													 camera::CameraObject::CameraType::SUB )},
	m_is_size_changed{ true } {
}

void Canvas::Update() {
	if (m_is_size_changed) {
		std::array<GLint, 4> m_viewport_size{};
		glGetIntegerv(GL_VIEWPORT, &m_viewport_size[0]);
		UpdateScreenXYWH(m_viewport_size);
		m_is_size_changed = false;
	}

	UiObject::Update();
}

void Canvas::Draw() {
    glDisable(GL_DEPTH_TEST);
	for (auto& child : GetChildren()) { child.second->Draw(); }
    glEnable(GL_DEPTH_TEST);
}

const glm::mat4 Canvas::GetUiCameraProjMatrix() const {
	return m_camera->GetProjectionMatrix();
}

}