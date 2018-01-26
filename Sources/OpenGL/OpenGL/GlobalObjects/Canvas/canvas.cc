#include "canvas.h"
#include <glm\gtc\matrix_transform.hpp>

namespace Canvas {

void Canvas::Update() {
	if (m_is_size_changed) {
		glGetIntegerv(GL_VIEWPORT, &m_viewport_size[0]);
		m_projection = GetOrthoProjectionMatrix(m_viewport_size);
		m_is_size_changed = false;
	}
}

void Canvas::Draw() {
	for (auto& child : GetChildren()) {
		child.second->Draw();
	}
}

const glm::mat4&& Canvas::GetOrthoProjectionMatrix(const std::array<GLint, 4>& input) const {
	return std::move(glm::ortho(0.f, (float)input[2], 0.f, (float)input[3]));
}

}