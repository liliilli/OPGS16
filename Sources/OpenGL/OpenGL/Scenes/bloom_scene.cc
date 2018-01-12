#include "bloom_scene.h"

BloomScene::BloomScene() :
    font{ "Resources/LSANS.TTF" } {

}

void BloomScene::HandleInput(GLFWwindow * const window) {
    Scene::HandleInput(window);
}

void BloomScene::Draw() {
    // Rendering

    // Draw Text (UI)
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    font.RenderText("BloomScene", { 25.f, 25.f }, .5f, { 1.f, 1.f, 1.f });
}

void BloomScene::Update() {


    // Camera Update
    camera.Refresh();
}

