#include "obj_script_1.h"            /*! Header file */

#include <glm\glm.hpp>
#include "..\..\..\GlobalObjects\Canvas\text.h"     /*! Canvas::Text */
#include "..\..\..\System\Manager\scene_manager.h"  /*! SceneManager */
#include "..\..\..\System\Manager\timer_manager.h"  /*! TimerManager */

#include "test_script_1.h"  /*! TestScript1 for Canvas */

ObjectScript1::ObjectScript1(Object& obj) : component::ScriptFrame{ obj } {
    Initiate();
    Start();
}

void ObjectScript1::DoWork(const size_t mode, const unsigned assigned_number) noexcept {
    m_moving    = true;
    m_mode      = mode;
    m_assigned_number = assigned_number;

    /*! If m_mode is 1, delay each object along with m_assigned_number */
    if (m_mode == 1) {
        m_moving = false;
        TimerManager::GetInstance().SetTimer(m_timer_1_delay, 300 * m_assigned_number,
                                             false, this, &ObjectScript1::Do_1Work);
    }
}

void ObjectScript1::Update() {
    if (m_moving) {
        switch (m_mode) {
        case 1:
            Proceed_1NormalLocal();
            break;
        case 3:
            Proceed_3WorldPosition();
            break;
        default: /*! Do nothing */ break;
        }
    }
}

void ObjectScript1::Proceed_1NormalLocal() {
    auto speed = 2.0f;
    auto local_pos = GetObject().GetLocalPosition();

    if (m_switch == 0) {        /*! Right */
        local_pos.x += speed;
        if (static_cast<int>(local_pos.x) >= 64) ++m_switch;
    }
    else if (m_switch == 1) {   /*! Down */
        local_pos.y -= speed;
        if (static_cast<int>(local_pos.y) <= -64) ++m_switch;
    }
    else if (m_switch == 2) {   /*! Left */
        local_pos.x -= speed;
        if (static_cast<int>(local_pos.x) <= -64) ++m_switch;
    }
    else if (m_switch == 3) {   /*! Up */
        local_pos.y += speed;
        if (static_cast<int>(local_pos.y) >= 64) ++m_switch;
    }
    else if (m_switch == 4) {   /*! Up */
        local_pos.x += speed;
        if (static_cast<int>(local_pos.x) >= 64) ++m_switch;
    }
    else if (m_switch == 5) {
        local_pos.y -= speed;
        if (static_cast<int>(local_pos.y) <= 0) ++m_switch;
    }
    else if (m_switch == 6) {
        local_pos.x -= speed;
        if (static_cast<int>(local_pos.x) <= 0) {
            m_switch = 0;
            m_moving = false;

            auto& canvas = SceneManager::GetInstance().GetPresentScene()->GetObject("GameCanvas");
            if (canvas) {
                TestScript1* canvas_script = canvas->GetComponent<TestScript1>();
                canvas_script->TriggerProcessFinish();
            }
        }
    }

    GetObject().SetLocalPosition(local_pos);
}

void ObjectScript1::Proceed_3WorldPosition() {
    auto pos = GetObject().GetWorldPosition();
    auto speed = 2.0f;

    if (m_switch == 0) {        /*! Right */
        pos.x += speed;
        if (static_cast<int>(pos.x) >= 128+64) ++m_switch;
    }
    else if (m_switch == 1) {   /*! Down */
        pos.y -= speed;
        if (static_cast<int>(pos.y) <= 112-64) ++m_switch;
    }
    else if (m_switch == 2) {   /*! Left */
        pos.x -= speed;
        if (static_cast<int>(pos.x) <= 128-64) ++m_switch;
    }
    else if (m_switch == 3) {   /*! Up */
        pos.y += speed;
        if (static_cast<int>(pos.y) >= 112+64) ++m_switch;
    }
    else if (m_switch == 4) {   /*! Up */
        pos.x += speed;
        if (static_cast<int>(pos.x) >= 128+64) ++m_switch;
    }
    else if (m_switch == 5) {
        pos.y -= speed;
        if (static_cast<int>(pos.y) <= 112) ++m_switch;
    }
    else if (m_switch == 6) {
        pos.x -= speed;
        if (static_cast<int>(pos.x) <= 128) {
            m_switch = 0;
            m_moving = false;

            auto& canvas = SceneManager::GetInstance().GetPresentScene()->GetObject("GameCanvas");
            if (canvas) {
                TestScript1* canvas_script = canvas->GetComponent<TestScript1>();
                canvas_script->TriggerProcessFinish();
            }
        }
    }

    GetObject().SetWorldPosition(pos);
}