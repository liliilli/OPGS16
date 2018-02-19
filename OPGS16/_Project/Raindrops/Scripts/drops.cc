#include "drops.h"      /*! Header file */
#include "rain_ui.h"    /*! RainUiManager */
#include "..\..\..\System\Object\object.h"          /*! Object */
#include "..\..\..\System\Manager\object_manager.h" /*! ObjectManager */
#include "..\..\..\System\Manager\scene_manager.h"  /*! SceneManager */
#include <glm\glm.hpp>
#include <random>

void Drops::Update() {
    auto& obj = GetObject();

    if (obj.GetWorldPosition().y < -16.f) {
        if (m_count <= 0)
            ObjectManager::GetInstance().Destroy(obj);
        else {
            std::random_device rng;
            auto x = rng() % 256;
            auto y = 232;

            obj.SetWorldPosition({ x, y, 0 });
            m_trigger = false;
            --m_count;
        }
    }
    else {
        auto pos = obj.GetWorldPosition();

        if (m_trigger) { /* Accelate */
            m_velocity += m_acc * m_mass;
            pos.y -= m_velocity;
        }
        else {
            m_mass = (std::random_device()() % 50) / 12.5f;
            m_trigger = true;
            m_velocity = 0.f;
        }

        obj.SetWorldPosition(pos);
    }
}

void Drops::ScoreUpTrigger() {
    RainUiManager* const i = SceneManager::GetInstance().GetPresentScene()
        ->GetObject("GameCanvas")->GetComponent<RainUiManager>();
    if (i) {
        i->AddScore(256);
        ObjectManager::GetInstance().Destroy(GetObject());
    }
}
