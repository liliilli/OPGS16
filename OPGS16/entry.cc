#include "application.h"
#include "System\Manager\resource_manager.h"

int main() {
	ResourceManager::GetInstance();	// Initiate resource list.
    auto& app = Application::getInstance("OPGS16");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	app.Initiate();
    app.Run();

    return 0;
}
