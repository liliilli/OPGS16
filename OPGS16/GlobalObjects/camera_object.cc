#include "camera_object.h"
#include <array>
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

namespace camera {

bool CameraObject::s_main_camera_initiated = false;

CameraObject::CameraObject(ViewType view_type, CameraType camera_type, bool _auto) :
	m_viewtype{ view_type }, m_cameratype{ camera_type } {
	/** Body */
	switch (m_viewtype) {
	case ViewType::ORTHO: InitiateOrthographicProjection(); break;
	case ViewType::PERSPECTIVE:
		break;
	}

	if (m_cameratype == CameraType::MAIN && s_main_camera_initiated)
		m_cameratype = CameraType::SUB;
}

CameraObject::~CameraObject() {
	if (m_cameratype == CameraType::MAIN)
		s_main_camera_initiated = false;
}

glm::mat4 CameraObject::GetProjectionMatrix() {
	return m_projection;
}

void CameraObject::InitiateOrthographicProjection() {
	std::array<GLint, 4> viewport_size{};
	glGetIntegerv(GL_VIEWPORT, &viewport_size[0]);

	m_projection = glm::ortho<float>(viewport_size[0], viewport_size[2],
							  viewport_size[1], viewport_size[3]);
}

void CameraObject::InitiatePerspectiveProjection() {
	std::array<GLint, 4> viewport_size{};
	glGetIntegerv(GL_VIEWPORT, &viewport_size[0]);

	auto fovy = glm::radians(50.f);
	auto ratio = static_cast<float>(viewport_size[2]) / viewport_size[3];
	m_projection = glm::perspective(fovy, ratio, 0.03f, 100.f);
}

}
