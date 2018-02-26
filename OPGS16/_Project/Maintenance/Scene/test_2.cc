#include "test_2.h"     /*! Header file */
#include <GL\glew.h>    /*! OpenGL functions */

void Maintenance_2::Draw() {
    glClearColor(.1f, .3f, .1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::Draw();
}

void Maintenance_2::Initiate() {

}