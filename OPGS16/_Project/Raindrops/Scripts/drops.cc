#include "drops.h" /*! Header file */
#include "..\..\..\System\Object\object.h"          /*! Object */
#include "..\..\..\System\Manager\object_manager.h" /*! ObjectManager */
#include <glm\glm.hpp>
#include <random>

void Drops::Update() {
    auto& obj = GetObject();

    if (obj.GetWorldPosition().y < -16.f) {
        if (count <= 0)
            ObjectManager::GetInstance().Destroy(obj);
        else {
            std::random_device rng;
            auto x = rng() % 256;
            auto y = rng() % 128 + 256;

            obj.SetWorldPosition({ x, y, 0 });
            --count;
        }
    }
    else {
        auto pos = obj.GetWorldPosition();
        pos.y -= 6.f;
        obj.SetWorldPosition(pos);
    }
}
