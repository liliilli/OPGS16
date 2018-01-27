#include "path_finding2d.h"
#include "..\Headers\canvas_components.h"
#include "..\GlobalObjects\Interface\i_originable.h"

PathFinding2D::PathFinding2D() {
	/** Body */
	InitUserInterface();
}

void PathFinding2D::HandleInput(GLFWwindow * const window) {
}

void PathFinding2D::Update() {
	Scene::Update();
}

void PathFinding2D::Draw() {
    glClearColor(.0f, .2f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void PathFinding2D::InitUserInterface() {
	InsertObject("Canvas", std::make_unique<Canvas::Canvas>());
	std::unique_ptr<Object>& canvas = GetObject("Canvas");
	//for (int i = 1; i <= 9; ++i) {
	//	Canvas::Text&& txt{ m_scene_name };
	//	txt.SetOrigin(static_cast<IOriginable::Origin>(i));
	//	txt.SetScaleValue(0.5f);
	//	txt.SetAlignment(IAlignable::Alignment::CENTER);

	//	std::string&& tag{ "txt" + std::to_string(i) };
	//	canvas->InitiateChild(std::move(tag), std::move(txt));
	//}

	// Menu
	Canvas::Image&& image{ "Resources/window.png" }; {
		image.SetOrigin(IOriginable::Origin::DOWN_RIGHT);
		image.SetLocalPosition({ -360.f, 80.f, 0 });
		image.SetScaleValue(80);
		image.SetScaleFactor({ 1, 2, 0 });

		std::string description =
			"Description\n"
			"(Z) Change to DFS\n"
			"(X) Change to BFS\n"
			"(C) Change to Dijkstra\n"
			"(V) Change to Bellman-ford\n"
			"(B) Change to A*";
		Canvas::Text&& text_1{ description, { -30, 70, 0 } };
		text_1.SetScaleValue(0.5f);
		image.InitiateChild("Description", std::move(text_1));
	}

	canvas->InitiateChild("Menu", std::move(image));
}

void PathFinding2D::InitShowingComponents() {
}

void PathFinding2D::DrawUi() {

}
