#include "canvas.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Canvas {
Canvas::Canvas() :
	m_camera{ camera::CameraObject::ViewType::ORTHO, camera::CameraObject::CameraType::SUB } {
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

glm::mat4 Canvas::GetUiCameraProjMatrix() {
	return m_camera.GetProjectionMatrix();
}

}