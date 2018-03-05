#include "test_1.h"     /*! Header file */

#include <glm\glm.hpp>

#include "..\..\..\GlobalObjects\camera_object.h"   /*! MainCameraObject */
#include "..\Object\game_canvas.h"                  /*! GameCanvas */
#include "..\Object\test_obj.h"                     /*! TestObject1 */

void Maintenance::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void Maintenance::Initiate() {
    auto main_camera = std::make_unique<MainCameraObject>();
    Instantiate("MainCamera", main_camera);

    auto canvas = std::make_unique<GameCanvas>();
    Instantiate("GameCanvas", canvas);

    auto obj_1 = std::make_unique<TestObject1>(1, 64.f); {
        obj_1->SetWorldPosition(glm::vec3{ 128, 112, 0 });
        Instantiate("Object", obj_1);

        auto ptr = GetObject("Object").get();
        ptr->SetRenderLayer(3);
        for (int i = 2; i <= 4; ++i) {
            ptr->Instantiate<TestObject1>("Object", i, 16.f * (5 - i));
            ptr = ptr->GetChild("Object");
            ptr->SetRenderLayer(3);
        }
    }
}