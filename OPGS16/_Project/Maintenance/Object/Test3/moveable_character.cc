#include "moveable_character.h"   /*! Header file */
#include "../../Scripts/Test3/mc_script.h"

#include "../../../System/Components/Public/camera.h"       /*! ::opgs16::component::Camera */
#include "../../../System/Components/Public/sprite_renderer.h"
#include "../../../System/Manager/Public/scene_manager.h"   /*! MSceneManager */
#include "../../../System/Shader/shader_wrapper.h"          /*! ShaderWrapper */

MoveableCharacter::MoveableCharacter(const int i, const float size) {
    SetScaleValue(size);

    {
        using opgs16::component::CSprite2DRenderer;
        AddComponent<CSprite2DRenderer>(*this, "128_" + std::to_string(i), "gQuad");
        m_wrapper = &GetComponent<CSprite2DRenderer>()->Wrapper();
        m_wrapper->InsertUniformValue<glm::mat4>("projection", glm::mat4{});
        m_wrapper->InsertUniformValue<float>("alpha", 1.0f);
    }

    AddComponent<McScript>(*this);
}

void MoveableCharacter::Render() {
    using opgs16::manager::MSceneManager;
    using opgs16::component::CSprite2DRenderer;

    const auto PVM = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix();
    m_wrapper->ReplaceUniformValue<glm::mat4>("projection", PVM);
    GetComponent<CSprite2DRenderer>()->RenderSprite();
}