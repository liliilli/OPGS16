#include "player.h" /*! Header file */
#include "..\..\..\System\Manager\input_manager.h"  /*! InputManager */
#include "..\..\..\System\Object\object.h"          /*! Object */
#include <glm\glm.hpp>

void Player::Update() {
    auto& input = InputManager::GetInstance();

    auto position = GetObject().GetFinalPosition();

    auto x_val = input.GetKeyValue("Horizontal");
    auto y_val = input.GetKeyValue("Vertical");
    if (x_val || y_val) {
        position.x += 2 * input.GetKeyValue("Horizontal");
        position.y += 2 * input.GetKeyValue("Vertical");
        GetObject().SetFinalPosition(position);
    }

}
