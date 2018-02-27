#include "test_2.h"     /*! Header file */
#include <GL\glew.h>    /*! OpenGL functions */

#include "..\..\..\GlobalObjects\camera_object.h"   /*! MainCameraObject */
#include "..\Object\Test2\game_canvas_2.h"          /*! GameCanvas2 */

void Maintenance_2::Draw() {
    glClearColor(.1f, .1f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::Draw();
}

void Maintenance_2::Initiate() {
    auto main_camera = std::make_unique<MainCameraObject>();
    Instantiate("MainCamera", main_camera);

    auto canvas = std::make_unique<GameCanvas2>();
    Instantiate("GameCanvas", canvas);
}