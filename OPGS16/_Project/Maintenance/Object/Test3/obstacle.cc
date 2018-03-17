#include "obstacle.h"
#include "../../Scripts/Test3/mc_script.h"

#include "../../../System/Components/Public/camera.h"       /*! ::opgs16::component::Camera */
#include "../../../System/Components/Public/sprite_renderer.h"
#include "../../../System/Manager/Public/scene_manager.h"   /*! MSceneManager */
#include "../../../System/Shader/shader_wrapper.h"          /*! ShaderWrapper */
#include "../../../../System/Components/Public/rigidbody_2d.h"

ObstacleBlock::ObstacleBlock() {
    SetScaleValue(8.f);
    using opgs16::component::CSprite2DRenderer;

    {
        AddComponent<CSprite2DRenderer>(*this, "128_3", "gQuad");
        m_wrapper = &GetComponent<CSprite2DRenderer>()->Wrapper();
        m_wrapper->InsertUniformValue<glm::mat4>("projection", glm::mat4{});
        m_wrapper->InsertUniformValue<float>("alpha", 1.0f);
    }

    {
        using opgs16::component::CRigidbody2D;
        AddComponent<CRigidbody2D>(*this);
        auto const rigidbody = GetComponent<CRigidbody2D>();
        rigidbody->AddCollider2D<collision::RectangleCollider2D>(-4.f, 4.f, 4.f, -4.f);
    }

    //AddComponent<McScript>(*this);
}

void ObstacleBlock::Render() {
    using opgs16::manager::MSceneManager;
    using opgs16::component::CSprite2DRenderer;

    m_wrapper->ReplaceUniformValue<glm::mat4>("projection",
        MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix());
    GetComponent<CSprite2DRenderer>()->RenderSprite();
}

void ObstacleBlock::OnCollisionEnter(opgs16::component::CRigidbody2D& collider) {
    const auto& pos = GetWorldPosition();
    char str[50] = { '\0', };
    char val[32] = { '\0', };
    strcat(str, "Collide with player ");
    strcat(str, itoa(static_cast<int>(pos.x), val, 10)); strcat(str, ", ");
    strcat(str, itoa(static_cast<int>(pos.y), val, 10)); strcat(str, ", ");
    strcat(str, itoa(static_cast<int>(pos.z), val, 10));
    opgs16::debug::PushLog(opgs16::debug::_internal::MsgType::INFO, str);
}
