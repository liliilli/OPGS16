#include "__boot.h"     /*! Header file */
#include <GL\glew.h>    /*! OpenGL functions */

#include "..\..\..\GlobalObjects\camera_object.h"   /*! MainCameraObject */

void __BOOT::Draw() {
    glClearColor(0, 0, 0, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void __BOOT::Initiate() {
    auto main_camera = std::make_unique<MainCameraObject>();
    Instantiate("Cam", main_camera);
}