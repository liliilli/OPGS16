//#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>

#include "application.h"

int main() {
    //_crtBreakAlloc = 180;

    auto& app = Application::getInstance("OPGS16");
	app.Initiate();
    app.Run();

    //_CrtDumpMemoryLeaks();
    return 0;
}
