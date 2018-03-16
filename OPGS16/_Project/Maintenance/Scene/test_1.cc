#include "test_1.h"     /*! Header file */

#include <glm/glm.hpp>

#include "../Object/Test1/game_canvas.h"         /*! GameCanvas */
#include "../Object/Test1/test_obj.h"            /*! TestObject1 */
#include "../Object/Test3/moveable_character.h"
#include "../../../GlobalObjects/camera_object.h"   /*! MainCameraObject */
#include "../../../System/Element/Public/object.h"
#include "../../../System/Components/Public/sprite_renderer.h"

void Maintenance::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Maintenance::Initiate() {
    auto l_set_render_layer = [](opgs16::element::CObject* const obj) {
        using Sprite2DRenderer = opgs16::component::CSprite2DRenderer;
        if (Sprite2DRenderer* renderer = obj->GetComponent<Sprite2DRenderer>(); renderer)
            renderer->SetRenderLayer(3);
    };

    Instantiate<GameCanvas>("GameCanvas");

    {
        auto obj_1 = Instantiate<MoveableCharacter>("Character", 1, 16.f);
        obj_1->SetWorldPosition(glm::vec3{ 128, 112, 0 });
        obj_1->Instantiate<MainCameraObject>("MainCamera");
        l_set_render_layer(obj_1);
    }

    {
        auto obj_1 = Instantiate<TestObject1>("Object", 1, 64.f);
        obj_1->SetWorldPosition(glm::vec3{ 128, 112, 0 });
        l_set_render_layer(obj_1);

        for (int i = 2; i <= 4; ++i) {
            obj_1 = obj_1->Instantiate<TestObject1>("Object", i, 16.f * (5 - i));
            l_set_render_layer(obj_1);
        }
    }
}