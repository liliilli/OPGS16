#include "test_obj.h"   /*! Header file */

#include "../../../System/Components/Public/camera.h"       /*! opgs16::component::Camera */
#include "../../../System/Components/Public/sprite_renderer.h"    /*! Sprite2DRenderer */
#include "../../../System/Manager/Public/scene_manager.h"   /*! SceneManager */
#include "../../../System/Shader/shader_wrapper.h"          /*! ShaderWrapper */
#include "../Scripts/obj_script_1.h"                        /*! ObjectScript1 */

TestObject1::TestObject1(const int i, const float size) {
    SetScaleValue(size);

    using opgs16::component::Sprite2DRenderer;
    AddComponent<Sprite2DRenderer>("128_" + std::to_string(i), "gQuad");
	m_wrapper = &GetComponent<Sprite2DRenderer>()->GetWrapper();
	m_wrapper->InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	m_wrapper->InsertUniformValue<float>("alpha", 1.0f);

    AddComponent<ObjectScript1>(*this);
}

void TestObject1::Render() {
    using opgs16::manager::SceneManager;
    auto PVM = SceneManager::Instance().PresentScene()->GetMainCamera()->GetPV() * GetModelMatrix();

    m_wrapper->ReplaceUniformValue<glm::mat4>("projection", PVM);
    //shader.ReplaceUniformValue("alpha", 1.0f);
    using opgs16::component::Sprite2DRenderer;
    GetComponent<Sprite2DRenderer>()->RenderSprite();
}