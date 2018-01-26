#include "path_finding2d.h"
#include "..\GlobalObjects\Interface\i_originable.h"

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

	using Origin = IOriginable::Origin;
	using Align = FontManager::FontAlignment;

	for (int i = 1; i <= 9; ++i) {
		font.RenderTextNew(m_scene_name,
			static_cast<Origin>(i),
			{0, 0}, { 1, 1, 1 }, Align::CENTER);
	}
}
