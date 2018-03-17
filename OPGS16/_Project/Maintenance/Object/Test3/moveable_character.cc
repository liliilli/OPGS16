#include "moveable_character.h"   /*! Header file */
#include "../../Scripts/Test3/mc_script.h"

#include "../../../System/Components/Public/camera.h"       /*! ::opgs16::component::Camera */
#include "../../../System/Components/Public/sprite_renderer.h"
#include "../../../System/Manager/Public/scene_manager.h"   /*! MSceneManager */
#include "../../../System/Shader/shader_wrapper.h"          /*! ShaderWrapper */
#include "../../../../System/Components/Public/rigidbody_2d.h"

MoveableCharacter::MoveableCharacter(const float size) {
    SetScaleValue(size);
    using opgs16::component::CSprite2DRenderer;
    using opgs16::component::CRigidbody2D;

    {
        AddComponent<CSprite2DRenderer>(*this, "128_1", "gQuad");
        m_wrapper = &GetComponent<CSprite2DRenderer>()->Wrapper();
        m_wrapper->InsertUniformValue<glm::mat4>("projection", glm::mat4{});
        m_wrapper->InsertUniformValue<float>("alpha", 1.0f);
    }

    {
        AddComponent<CRigidbody2D>(*this);
        auto const rigidbody = GetComponent<CRigidbody2D>();
        rigidbody->ActivatePhysics(true);
        auto& ref = rigidbody->AddCollider2D<collision::RectangleCollider2D>(-8.f, 8.f, 8.f, -8.f);
        ref->SetCollisionLayerIndex(1);
    }

    AddComponent<McScript>(*this);
}

void MoveableCharacter::Render() {
    using opgs16::manager::MSceneManager;
    using opgs16::component::CSprite2DRenderer;

    const auto pvm = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix();
    m_wrapper->ReplaceUniformValue<glm::mat4>("projection", pvm);
    GetComponent<CSprite2DRenderer>()->RenderSprite();
}
