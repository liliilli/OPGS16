#include "start.h"
#include "..\application.h"
#include "..\GlobalObjects\Canvas\image.h"
#include "bloom_scene.h"
#include "primitive_proc.h"
#include "path_finding2d.h"

Start::Start() :
    font{"Resources/LSANS.TTF"} {
    glEnable(GL_DEPTH_TEST);

	objects[0] = std::make_unique<Canvas::Image>("Resources/window.png");
}

void Start::InitShaders() {

}

void Start::HandleInput(GLFWwindow* const window) {
    if (DoesKeyPressed(window, GLFW_KEY_A)) {

    }
    else if (DoesKeyPressed(window, GLFW_KEY_B)) {
        Application::getInstance().ReplaceScene((PrimProc*)nullptr);
    }
    else if (DoesKeyPressed(window, GLFW_KEY_C)) {
        Application::getInstance().ReplaceScene((BloomScene*)nullptr);
    }
	else if (DoesKeyPressed(window, GLFW_KEY_D)) {
		Application::getInstance().ReplaceScene((PathFinding2D*)nullptr);
	}

    Scene::HandleInput(window);
}

void Start::Draw() {
    glClearColor(.0f, .0f, .0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objects[0]->Draw();

    DrawText();
}

void Start::DrawText() {
    glDisable(GL_DEPTH_TEST);

    /** String rendering */
    font.RenderText("OpenGL Tutorial Samples", { 25, 480 - 48 }, 1.f, { 1, 1, 1 });

    std::string text = "A : Terrain Tesslation\n"
        "B : Flapping Carpet\n"
        "C : Bloom\n"
		"D : PathFinding2D";
    font.RenderText(text, { 25, 240 }, .5f, { 1, 1, 1 });

    std::string copyright = "Copyright (c) 2018, Jongmin Yun All rights reserved.";
    font.RenderText(copyright, { 16, 16 }, .25f, { 1, 1, 1 });

    glEnable(GL_DEPTH_TEST);
}

void Start::Update() {
}
