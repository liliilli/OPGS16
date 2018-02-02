#include <iostream>
#include <memory>
#include <functional>
#include "stb_image.h"

#include "application.h"
//#include "camera.h"
//#include "constant.h"
//#include "helper.h"
//#include "main.h"
//#include "model.h"
//#include "shader.h"
//#include "texture.h"
//

int main() {
    auto& app = Application::getInstance("Hello World");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	app.Initiate();
    app.Run();

    glfwTerminate();
    return 0;
}
