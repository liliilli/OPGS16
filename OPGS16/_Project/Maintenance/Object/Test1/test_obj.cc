#include "test_obj.h"   /*! Header file */

#include "../../../System/Components/Public/camera.h"       /*! ::opgs16::component::Camera */
#include "../../../System/Components/Public/animator.h"     /*! ::opgs16::component::CAnimator */
/*! ::opgs16::component::CSprite2DRenderer */
#include "../../../System/Components/Public/sprite_renderer.h"
#include "../../../System/Manager/Public/scene_manager.h"   /*! MSceneManager */
#include "../../../System/Shader/shader_wrapper.h"          /*! ShaderWrapper */
#include "../../Scripts/Test1/obj_script_1.h"               /*! ObjectScript1 */

TestObject1::TestObject1(const int i, const float size) {
    SetScaleValue(size);

    {
        using opgs16::component::CSprite2DRenderer;
        AddComponent<CSprite2DRenderer>(*this, "128_" + std::to_string(i), "gQuad");
        m_wrapper = &GetComponent<CSprite2DRenderer>()->Wrapper();
        m_wrapper->InsertUniformValue<glm::mat4>("projection", glm::mat4{});
        m_wrapper->InsertUniformValue<float>("alpha", 1.0f);

        using opgs16::component::CAnimator;
        AddComponent<CAnimator>(*this, *GetComponent<CSprite2DRenderer>(), opgs16::Switch::ON);
    }

    AddComponent<ObjectScript1>(*this);
}

void TestObject1::Render() {
    using opgs16::manager::MSceneManager;
    auto PVM = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix();

    m_wrapper->ReplaceUniformValue<glm::mat4>("projection", PVM);
    using opgs16::component::CSprite2DRenderer;
    GetComponent<CSprite2DRenderer>()->RenderSprite();
}