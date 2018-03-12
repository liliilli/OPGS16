#include "test_script_1.h"            /*! Header file */

#include <glm/glm.hpp>
#include "../../../GlobalObjects/Canvas/text.h"     /*! Canvas::Text */
#include "../../../System/Manager/Public/scene_manager.h"  /*! MSceneManager */
#include "../../../System/Manager/Public/sound_manager.h"  /*! MSoundManager */
#include "../../../System/Manager/Public/timer_manager.h"  /*! MTimerManager */

#include "obj_script_1.h"   /*! ObjectScript1 for TestObject1 */

using opgs16::manager::MSceneManager;
using opgs16::element::CObject;

TestScript1::TestScript1(CObject& obj) : opgs16::component::CScriptFrame{ obj } {
    Initiate();
    Start();
}

void TestScript1::Start() {
    SetBreakTimer();

    auto& sound_manager = opgs16::manager::MSoundManager::Instance();
    sound_manager.CreateSound("Music1");
    sound_manager.PlaySound("Music1");
}

void TestScript1::Update() {
    if (!m_is_break) {
        switch (m_sequence) {
        default: /*! Do nothing */ break;
        case Sequence::_1_NORMAL_LOCAL:
            Proceed_1NormalLocal();
            break;
        case Sequence::_2_DEPENDENT_WORLD:
            Proceed_2DependentWorld();
            break;
        case Sequence::_3_INDEPENDENT_WORLD:
            Proceed_3WorldPosition();
            break;
        case Sequence::_4_ALPHA_BLENDING_TEST:
            Proceed_4AlphaBlending();
            break;
        case Sequence::_5_SCALING_TEST:
            Proceed_5ScalingTest();
            break;
        }
    }
    else {
        switch (m_sequence) {
        default: /*! Do nothing */ break;
        case Sequence::_1_NORMAL_LOCAL: {
            if (m_finished_obj == 4) {
                m_finished_obj = 0;
                SetBreakTimer();
            }
        }   break;
        case Sequence::_3_INDEPENDENT_WORLD: {
            if (m_finished_obj == 1) {
                m_finished_obj = 0;
                ResetObjectProperties();
                opgs16::manager::MTimerManager::Instance().DetachTimer(m_timer_break);
                SetBreakTimer();
            }
        }   break;
        }
    }
}

void TestScript1::SetBreakTimer() {
    opgs16::manager::MTimerManager::Instance().SetTimer(m_timer_break, 1'500, false,
                                         this, &TestScript1::Resume);
}

void TestScript1::Resume() {
    m_is_break  = false;
    m_switch    = 0;
    m_sequence  = static_cast<Sequence>(static_cast<size_t>(m_sequence) + 1);

    ObjectPropertiesReset();
    ChangeText();

    switch (m_sequence) {
    case Sequence::_3_INDEPENDENT_WORLD: opgs16::manager::MTimerManager::Instance().SetTimer(m_timer_third_test, 500, true,
                                             this, &TestScript1::ToggleObjectProperties);
        break;
    }
}

void TestScript1::Proceed_1NormalLocal() {
    auto scene = MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get objects */
        std::vector<CObject*> obj_list{ scene->GetObject("Object").get() };
        while (*obj_list.rbegin() != nullptr) {
            auto object = *obj_list.rbegin();
            obj_list.emplace_back(object->GetChild("Object"));
        }
        obj_list.erase(--obj_list.cend());

        /*! Propagate them to do work */
        auto obj_number = 0;
        for (auto& obj_raw : obj_list) {
            ObjectScript1* script = obj_raw->GetComponent<ObjectScript1>();
            script->DoWork(1, obj_number);
            ++obj_number;
        }
    }

    m_is_break = true;
}

void TestScript1::Proceed_2DependentWorld() {
    auto scene = MSceneManager::Instance().PresentScene();
    auto speed = 2.0f;

    if (scene) {
        auto& object = scene->GetObject("Object");
        auto pos = object->GetWorldPosition();
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
                ++m_switch;
                SetBreakTimer();
            }
        }

        object->SetWorldPosition(pos);
    }
}

void TestScript1::Proceed_3WorldPosition() {
    auto scene = opgs16::manager::MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get 1-level object */
        auto& root_obj = scene->GetObject("Object");
        if (root_obj) {
            ObjectScript1* script = root_obj->GetComponent<ObjectScript1>();
            script->DoWork(3, 0);
        }
    }

    m_is_break = true;
}

void TestScript1::ObjectPropertiesReset() {
    /*! Reset all things */
    switch (m_sequence) {
    case Sequence::_3_INDEPENDENT_WORLD: {
        auto scene = MSceneManager::Instance().PresentScene();

        bool translate_dependency = true;
        auto object = scene->GetObject("Object").get();
        while (object != nullptr) {
            object->SetSucceedingPositionFlag(translate_dependency);

            /*! Set translate succeeding flag */
            object = object->GetChild("Object");
            translate_dependency = !translate_dependency;
        }
    }   break;
    default: /*! Do nothing */ break;
    }
}

void TestScript1::ChangeText() {
    /*! Text statements change */
    auto text = static_cast<::canvas::Text*>(GetObject().GetChild("Text"));
    switch (m_sequence) {
    case Sequence::_1_NORMAL_LOCAL:
        text->SetText(u8"Look for local translation of\ncenter boxes.");
        break;
    case Sequence::_2_DEPENDENT_WORLD:
        text->SetText(u8"Look for world translation of\nblue box and its children.");
        break;
    case Sequence::_3_INDEPENDENT_WORLD:
        text->SetText(u8"Look for weird but intentional\nworld translation of center boxes.");
        break;
    case Sequence::_4_ALPHA_BLENDING_TEST:
        text->SetText(u8"Look for\nblinking boxes.");
        break;
    case Sequence::_5_SCALING_TEST:
        text->SetText(u8"Look for scaling\neach box separately.");
        break;
    case Sequence::_6_ROTATION_TEST:
        text->SetText(u8"Look for\nappropriate rotation");
    default: /*! Do nothing */ break;
    }
}

void TestScript1::ToggleObjectProperties() {
    auto scene = MSceneManager::Instance().PresentScene();
    auto object = scene->GetObject("Object").get();
    while (object != nullptr) {
        auto flag = object->GetSucceedingPositionFlag();
        object->SetSucceedingPositionFlag(!flag);
        object = object->GetChild("Object");
    }
}

void TestScript1::ResetObjectProperties() {
    auto scene = MSceneManager::Instance().PresentScene();
    auto object = scene->GetObject("Object").get();
    auto root_object = object;

    while (object != nullptr) {
        object->SetSucceedingPositionFlag(true);
        object = object->GetChild("Object");
    }

    root_object->SetWorldPosition({ 128, 112, 0 });
}

void TestScript1::Proceed_4AlphaBlending() {
    auto scene = MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get objects */
        std::vector<CObject*> obj_list{ scene->GetObject("Object").get() };
        while (*obj_list.rbegin() != nullptr) {
            auto object = *obj_list.rbegin();
            obj_list.emplace_back(object->GetChild("Object"));
        }
        obj_list.erase(--obj_list.cend());

        /*! Propagate them to do work */
        auto obj_number = 0;
        for (auto& obj_raw : obj_list) {
            ObjectScript1* script = obj_raw->GetComponent<ObjectScript1>();
            script->DoWork(4, obj_number);
            ++obj_number;
        }
    }

    opgs16::manager::MTimerManager::Instance().SetTimer(m_timer_4_interval, 6'000, false,
                                         this, &TestScript1::OnTrigger4Interval);
    m_is_break = true;
}

void TestScript1::OnTrigger4Interval() {
    auto scene = MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get Objects and let them stop their own timers */
        auto object = scene->GetObject("Object").get();
        while (object != nullptr) {
            ObjectScript1* script = object->GetComponent<ObjectScript1>();
            script->StopAllTimers();
            object = object->GetChild("Object");
        }
    }
    /*! Move to next level... */
    SetBreakTimer();
}

void TestScript1::Proceed_5ScalingTest() {
    auto scene = MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get objects */
        std::vector<CObject*> obj_list{ scene->GetObject("Object").get() };
        while (*obj_list.rbegin() != nullptr) {
            auto object = *obj_list.rbegin();
            obj_list.emplace_back(object->GetChild("Object"));
        }
        obj_list.erase(--obj_list.cend());

        /*! Propagate them to do work */
        auto obj_number = 0;
        for (auto& obj_raw : obj_list) {
            ObjectScript1* script = obj_raw->GetComponent<ObjectScript1>();
            script->DoWork(5, obj_number);
            ++obj_number;
        }
    }

    opgs16::manager::MTimerManager::Instance().SetTimer(m_timer_5_interval, 6'000, false,
                                         this, &TestScript1::OnTrigger5Interval);
    m_is_break = true;
}

void TestScript1::OnTrigger5Interval() {
    auto scene = MSceneManager::Instance().PresentScene();
    if (scene) {
        /*! Get Objects and let them stop their own timers */
        auto object = scene->GetObject("Object").get();
        while (object != nullptr) {
            ObjectScript1* script = object->GetComponent<ObjectScript1>();
            script->StopAllTimers();
            object = object->GetChild("Object");
        }
    }
    /*! Move to next level... */
    SetBreakTimer();
}
