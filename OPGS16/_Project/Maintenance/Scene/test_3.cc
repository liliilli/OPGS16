#include "test_3.h"                                 /*! Header file */
#include "../Object/Test3/moveable_character.h"
#include "../../../GlobalObjects/camera_object.h"   /*! MainCameraObject */
#include "../../../System/Element/Public/object.h"
#include "../../../System/Components/Public/sprite_renderer.h"
#include "../Object/Test3/empty_object.h"

#include <chrono>
#include <random>
#include "../Object/Test3/obstacle.h"

void Maintenance3::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Maintenance3::Initiate() {
    auto l_set_render_layer = [](opgs16::element::CObject* const obj, const unsigned i) {
        using Sprite2DRenderer = opgs16::component::CSprite2DRenderer;
        if (Sprite2DRenderer* renderer = obj->GetComponent<Sprite2DRenderer>(); renderer)
            renderer->SetRenderLayer(i);
    };

    /*! Moveable and Physics influenced object */
    {
        auto obj_1 = Instantiate<MoveableCharacter>("Character", 16.f);
        obj_1->SetWorldPosition(glm::vec3{ 0, 1024, 0 });
        obj_1->Instantiate<MainCameraObject>("MainCamera");
        l_set_render_layer(obj_1, 2);
    }

    /*! Map information */
    char map[] = {
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1, 1,1,0,1,1,0,0,1, 1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0, 1,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,0, 0,0,1,1,1,0,0,0, 0,0,1,1,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0, 0,0,1,1,1,1,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,0, 0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,1,1,0,0, 0,0,0,0,0,0,1,1,
        1,1,1,1,1,1,1,1, 1,1,0,1,1,0,0,1, 1,1,1,1,1,0,0,0, 0,0,0,0,0,0,1,1,
    };

    /*! Obstacles */
    {
        auto empty = Instantiate<EmptyObject>("Obstacles");
        auto rng = std::default_random_engine();
        rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

        for (auto i = 1000; i >= 0; i -= 120) {
            const auto lim = (1020 - i) / 120;
            for (auto j = 0; j < lim; ++j) {
                auto obj = empty->Instantiate<ObstacleBlock>("Obstacle");
                int x = (rng() % 240) - 120;
                int y = (rng() % 30) - 15 + i;
                obj->SetWorldPosition({ x, y, 0 });
            }
        }
    }
}