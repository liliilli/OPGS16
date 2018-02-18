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

void RainScript::Start() {
    for (auto i = 0; i < 256; ++i) {
        auto x = m_rng() % 256;
        auto y = m_rng() % 228 + 228;

        auto drops = std::make_unique<RainDrop>("Test");
        drops->SetWorldPosition(glm::vec3{ x, y, 0 });
        GetObject().Instantiate<RainDrop>("Drops", drops);
    }
}

void RainScript::Update() {}