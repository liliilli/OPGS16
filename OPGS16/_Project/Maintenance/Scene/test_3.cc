#include "test_3.h"                                 /*! Header file */
#include "../Object/Test3/moveable_character.h"
#include "../../../GlobalObjects/camera_object.h"   /*! MainCameraObject */
#include "../../../System/Element/Public/object.h"
#include "../../../System/Components/Public/sprite_renderer.h"

void Maintenance3::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Maintenance3::Initiate() {
    auto obj_1 = std::make_unique<MoveableCharacter>(1, 16.f); {
        obj_1->SetWorldPosition(glm::vec3{ 128, 112, 0 });
        Instantiate("Object", obj_1);

        auto ptr = GetObject("Object").get();
        auto main_camera = std::make_unique<MainCameraObject>();
        ptr->Instantiate("MainCamera", main_camera);

        using Sprite2DRenderer = opgs16::component::CSprite2DRenderer;
        Sprite2DRenderer* renderer = ptr->GetComponent<Sprite2DRenderer>();
        renderer->SetRenderLayer(3);
    }
}