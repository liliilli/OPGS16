#include "rain_script.h"            /*! Header file */

#include <glm\glm.hpp>
#include "..\Object\rain_drop.h"    /*! RainDrop */
#include "..\..\..\System\Manager\input_manager.h"  /*! InputManager */
#include "..\..\..\System\Manager\scene_manager.h"  /*! SceneManager */

RainScript::RainScript(Object& obj) :
    component::ScriptFrame{ obj }, m_input{ InputManager::GetInstance() } {
    Initiate();
    Start();
}

void RainScript::Start() {}

void RainScript::Update() {
    if (!m_start) {
        if (m_input.GetKeyValue("Vertical") == 1.0f) {
            for (auto i = 0; i < 512; ++i) {
                auto x = m_rng() % 256;
                auto y = 232;

                auto drops = std::make_unique<RainDrop>("Test");
                drops->SetWorldPosition(glm::vec3{ x, y, 0 });
                GetObject().Instantiate<RainDrop>("Drops", drops);
            }

            m_start = true;
        }
    }
}