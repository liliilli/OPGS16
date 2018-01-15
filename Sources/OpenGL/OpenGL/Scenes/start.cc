#include "start.h"

Start::Start() :
    font{"Resources/LSANS.TTF"} {
    
}

void Start::HandleInput(GLFWwindow* const window) {
    Scene::HandleInput(window);
}

void Start::Draw() {
    /** String rendering */
    font.RenderText("OpenGL Tutorial Samples", { 25, 480 - 48 }, 1.f, { 1, 1, 1 });

    std::string text = "1 : Terrain Tesslation\n" 
        "2 : Flapping Carpet\n" 
        "3 : Bloom";
    font.RenderText(text, { 25, 240 }, .5f, { 1, 1, 1 });
}

void Start::Update() {
    
}
