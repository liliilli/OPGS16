#include "mc_script.h"

#include "../../../../System/Manager/Public/input_manager.h"
#include "../../../../System/Manager/Public/scene_manager.h"
#include "../../../../System/Components/Public/camera.h"
#include "../../../../System/Components/Public/rigidbody_2d.h"

namespace {
constexpr float maximum_speed{ 90.f };
constexpr float offset{ 8.f };
constexpr float dec_velocity{ 10.f };
} /*! unnamed namespace */

using opgs16::manager::MSceneManager;
using opgs16::element::CObject;

McScript::McScript(CObject& obj) : opgs16::component::CScriptFrame{ obj } {
    Initiate();
    Start();
}

void McScript::Start() {
    m_rigidbody = GetObject().GetComponent<opgs16::component::CRigidbody2D>();
}

void McScript::Update() {
    const auto& input = opgs16::manager::MInputManager::Instance();

    /*! Accelation */
    if (const auto x_val = input.GetKeyValue("Horizontal"); x_val) {
        if (auto& speed = m_rigidbody->Velocity().x; abs(speed += x_val * offset) >= maximum_speed)
            speed = (x_val > 0) ? maximum_speed : -maximum_speed;

        //m_rigidbody->SetStable(false);
    }
    else { /*! Break */
        auto& spd_x = m_rigidbody->Velocity().x;
        if (spd_x > 0) spd_x -= dec_velocity; else if (spd_x < 0) spd_x += dec_velocity;
        if ((spd_x > 0 && spd_x < dec_velocity) || (spd_x < 0 && spd_x > -dec_velocity))
            spd_x = 0.f;
    }

    /*! Jump */
    if (!m_jump && input.IsKeyPressed("Vertical")) {
        m_rigidbody->Velocity().y = maximum_speed;
        m_jump = true;
    }
    else if (m_jump && input.IsKeyReleased("Vertical"))
        m_jump = false;

    /*! Camera set */
    auto& obj = GetObject();
    if (auto camera = obj.GetChild("MainCamera"); camera) {
        using opgs16::component::CCamera;
        camera->GetComponent<CCamera>()->SetWorldPosition(obj.GetWorldPosition());
    }
}
