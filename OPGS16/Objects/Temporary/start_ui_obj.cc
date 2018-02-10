#include "start_ui_obj.h"                       /*! Header File */
#include "..\..\System\Manager\input_manager.h" /*! InputManager */

StartUiManager::StartUiManager() :
    input_manager{ InputManager::GetInstance() } {
}

void StartUiManager::Update() {
    if (input_manager.IsKeyPressed("StartD")) {

    }
}
