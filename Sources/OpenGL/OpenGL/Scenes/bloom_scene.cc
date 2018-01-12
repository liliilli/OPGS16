#include "bloom_scene.h"
#include "..\Objects\wood_box.h"
#include "..\Objects\quad_floor.h"
#include "..\Objects\light_box.h"

BloomScene::BloomScene() : font{ "Resources/LSANS.TTF" } { 
    /** Set shader up */
    using Type = helper::ShaderNew::Type;
    shader.SetShader(Type::VS, "Shaders/Bloom/normal.vert")
        .SetShader(Type::FS, "Shaders/Bloom/normal.frag")
        .Link();

    /** Create Instance to use */

    /** Boxes */
    objects[0] = std::make_unique<WoodBox>();
    objects[0]->SetPosition(glm::vec3{ 0, 0, 0 });

    objects[1] = std::make_unique<WoodBox>();
    objects[1]->SetPosition(glm::vec3{ 2, 0, -2 });

    objects[2] = std::make_unique<WoodBox>();
    objects[2]->SetPosition(glm::vec3{ -1, 2, -1 });

    objects[3] = std::make_unique<WoodBox>();
    objects[3]->SetPosition(glm::vec3{ -3, 0, -2 });

    /** Floor */
    objects[4] = std::make_unique<QuadFloor>();
    objects[4]->SetPosition({ 0, -.5f, 0 });
    objects[4]->SetAngle(-90.0f);
    objects[4]->SetRotationFactor({ 1, 0, 0 });
    objects[4]->SetScaleValue(4.0f);

    /** Light boxes */
}

void BloomScene::HandleInput(GLFWwindow * const window) {
    Scene::HandleInput(window);
}

void BloomScene::Draw() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering
    shader.Use();
    auto view = camera.GetViewMatrix();
    shader.SetVecMatrix4f("uView", view);
    shader.SetVecMatrix4f("uProjection", camera.GetProjection());
    shader.SetVec3f("uCameraPos", camera.GetPosition());

    /** Set light to shader */ {
        int i = 0;
        for (auto& light : radiant_objects) {

        }
    }

    objects[0]->Draw(shader);
    objects[1]->Draw(shader);
    objects[2]->Draw(shader);
    objects[3]->Draw(shader);

    objects[4]->Draw(shader);

    // Draw Text (UI)
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    font.RenderText("BloomScene", { 25.f, 25.f }, .5f, { 1.f, 1.f, 1.f });
}

void BloomScene::Update() {
    for (auto& object : objects) {
        object.second->Update();
    }

    // Camera Update
    camera.Refresh();
}

