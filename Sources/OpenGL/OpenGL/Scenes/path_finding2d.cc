#include "path_finding2d.h"

void PathFinding2D::HandleInput(GLFWwindow * const window) {
}

void PathFinding2D::Update() {
}

void PathFinding2D::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawUi();
}

void PathFinding2D::InitUserInterface() {
}

void PathFinding2D::InitShowingComponents() {
}

void PathFinding2D::DrawUi() {
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	font.RenderText(m_scene_name, { 25.f, 25.f }, 1.0f, { 1.f, 1.f, 1.f });
}
