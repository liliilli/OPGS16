#include "application.h"

int main() {
    auto& app = Application::getInstance("OPGS16");
	app.Initiate();
    app.Run();

    return 0;
}
