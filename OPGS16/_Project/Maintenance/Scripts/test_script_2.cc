#include "test_script_2.h"  /*! Header file */

#include "../../../System/Manager/Public/input_manager.h"  /*! InputManager */
#include "..\..\..\System\Manager\scene_manager.h"  /*! SceneManager */
#include "..\Scene\test_2.h"    /*! Maintenance_2 scene */

TestScript2::TestScript2(Object& obj) :
    component::ScriptFrame(obj), m_input{ opgs16::manager::InputManager::Instance() } {
}

void TestScript2::Update() {
    if (m_input.IsKeyPressed("Horizontal")) {
        SceneManager::GetInstance().ReplaceScene<Maintenance_2>();
    }
}
