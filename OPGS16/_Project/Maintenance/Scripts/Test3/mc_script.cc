#include "mc_script.h"

#include "../../../../GlobalObjects/Canvas/text.h"
#include "../../../../System/Manager/Public/input_manager.h"
#include "../../../../System/Manager/Public/scene_manager.h"
#include "../../../../System/Components/Public/camera.h"

using opgs16::manager::MSceneManager;
using opgs16::element::CObject;

McScript::McScript(CObject& obj) : opgs16::component::CScriptFrame{ obj } {
    Initiate();
    Start();
}

void McScript::Start() {}

void McScript::Update() {
    const auto& input = opgs16::manager::MInputManager::Instance();

    auto& obj = GetObject();
    auto position = obj.GetWorldPosition();

    auto x_val = input.GetKeyValue("Horizontal");
    auto y_val = input.GetKeyValue("Vertical");
    if (x_val || y_val) {
        position.x += 2 * input.GetKeyValue("Horizontal");
        position.y += 2 * input.GetKeyValue("Vertical");
        obj.SetWorldPosition(position);

        using opgs16::component::CCamera;
        if (CCamera* camera = obj.GetComponent<CCamera>(); camera) {
            camera->SetWorldPosition(obj.GetWorldPosition());
        }
    }
}
