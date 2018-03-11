#include "test_script_2.h"  /*! Header file */

#include "../../../System/Manager/Public/input_manager.h"  /*! InputManager */
#include "../../../System/Manager/Public/scene_manager.h"  /*! SceneManager */
#include "../Scene/test_2.h"    /*! Maintenance_2 scene */

TestScript2::TestScript2(opgs16::element::Object& obj) :
    ScriptFrame(obj), m_input{ opgs16::manager::InputManager::Instance() } {
}

void TestScript2::Update() {
    if (m_input.IsKeyPressed("Horizontal")) {
        opgs16::manager::SceneManager::Instance().ReplaceScene<Maintenance_2>();
    }
}
