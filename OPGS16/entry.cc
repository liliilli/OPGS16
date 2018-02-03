#include "application.h"

int main() {
    auto& app = Application::getInstance("Hello World");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	app.Initiate();
    app.Run();

    glfwTerminate();
    return 0;
}
