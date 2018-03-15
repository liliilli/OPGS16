#include "test_script_2.h"  /*! Header file */

#include "../../../System/Manager/Public/input_manager.h"  /*! MInputManager */
#include "../../../System/Manager/Public/scene_manager.h"  /*! MSceneManager */
#include "../../Scene/test_2.h"    /*! Maintenance_2 scene */

TestScript2::TestScript2(opgs16::element::CObject& obj) :
    CScriptFrame(obj), m_input{ opgs16::manager::MInputManager::Instance() } {
}

void TestScript2::Update() {
    if (m_input.IsKeyPressed("KeyD")) {
        opgs16::manager::MSceneManager::Instance().ReplaceScene<Maintenance_2>();
    }
}
