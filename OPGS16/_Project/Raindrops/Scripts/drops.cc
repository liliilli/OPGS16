#include "drops.h" /*! Header file */
#include "..\..\..\System\Object\object.h"          /*! Object */
#include "..\..\..\System\Manager\object_manager.h" /*! ObjectManager */
#include <glm\glm.hpp>
#include <random>

void Drops::Update() {
    auto& obj = GetObject();

    if (obj.GetFinalPosition().y < -16.f) {
        std::random_device rng;
        auto x = rng() % 256;
        auto y = rng() % 128 + 256;

        obj.SetLocalPosition({ x, y, 0 });
    }
    else {
        auto pos = obj.GetLocalPosition();
        pos.y -= 3.f;
        obj.SetLocalPosition(pos);
    }
}
