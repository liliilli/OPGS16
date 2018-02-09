#include "path_finding2d.h"
#include "..\Headers\canvas_components.h"
#include "..\GlobalObjects\Interface\i_originable.h"

PathFinding2D::PathFinding2D() {
	/** Body */
	InitUserInterface();
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
	auto canvas = static_cast<Canvas::Canvas*>(GetObject("Canvas").get());

	//// Menu
	//Canvas::Image&& image{ "Resources/window.png", canvas }; {
	//	image.SetOrigin(IOriginable::Origin::DOWN_RIGHT);
	//	image.SetImageSize(192, 240);
	//	image.SetLocalPosition(glm::vec3{ -96-32, 152, 0 });

	//	std::string description =
	//		"Description\n"
	//		"(Z) Change to DFS\n"
	//		"(X) Change to BFS\n"
	//		"(C) Change to Dijkstra\n"
	//		"(V) Change to Bellman-ford\n"
	//		"(B) Change to A*";
	//	Canvas::Text&& text_1{ description, { 16, -32, 0 } };
	//	text_1.SetOrigin(IOriginable::Origin::UP_LEFT);
	//	text_1.SetScaleValue(0.5f);
	//	image.InitiateChild("Description", std::move(text_1));
	//}

	//canvas->InitiateChild("Menu", std::move(image));

	//// Scene name
	//Canvas::Text&& name{ "Scene : PathFinding2D", {32, 32, 0} }; {
	//	name.SetOrigin(IOriginable::Origin::DOWN_LEFT);
	//	name.SetFontSize(16);
	//}
	//canvas->InitiateChild("Title", std::move(name));
}

void PathFinding2D::InitShowingComponents() {
}

void PathFinding2D::DrawUi() {

}
