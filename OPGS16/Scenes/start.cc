#include "start.h"
#include "..\application.h"
#include "..\Headers\canvas_components.h"
#include "path_finding2d.h"

Start::Start() {
	auto canvas = std::make_unique<Canvas::Canvas>(); {
		auto image = std::make_unique<Canvas::Image>("Test", canvas);
		image->SetImageSize(256, 224);
		canvas->InitiateChild("Image", image);
	}

	auto image = canvas->GetChild("Image");
	auto image_temp = static_cast<Canvas::Image*>(image);
	image->InitiateChild<Canvas::Text>("Txt1", "In Messsage Hello world", glm::vec3{ -48, 0, 0 });
	image->InitiateChild<Canvas::Text>("Txt2", "In Messsage Hello world", glm::vec3{ -48, 192, 0 });
	image->InitiateChild<Canvas::Text>("Txt3", "In Messsage Hello world", glm::vec3{ -48, -192, 0 });

    std::string text = "A : Terrain Tesslation\n"
        "B : Flapping Carpet\n"
        "C : Bloom\n"
		"D : PathFinding2D";
	auto txt_2 = std::make_unique<Canvas::Text>(text, glm::vec3{ 25, 0, 0 }); {
		txt_2->SetOrigin(IOriginable::Origin::CENTER_LEFT);
		txt_2->SetFontSize(16);
	}
	canvas->InitiateChild("Txt2", txt_2);

    std::string copy = "Copyright (c) 2018, Jongmin Yun All rights reserved";
	auto txt_3 = std::make_unique<Canvas::Text>(copy, glm::vec3{16, 16, 0}); {
		txt_3->SetOrigin(IOriginable::Origin::DOWN_LEFT);
		txt_3->SetFontSize(8);
	}
	canvas->InitiateChild("Txt3", txt_3);

	InsertObject("Canvas", canvas);
}

void Start::Draw() {
    glClearColor(.2f, .0f, .0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Start::Update() {
	Scene::Update();
}
