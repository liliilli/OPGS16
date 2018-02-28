#include "__nowhere.h"  /*! Header file */
#include <GL\glew.h>    /*! OpenGL functions */

void __NOTHING::Draw() {
    glClearColor(0, 0, 0, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::Draw();
}

void __NOTHING::Initiate() {

}