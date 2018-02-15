#include "object_collision.h"
#include "..\..\System\Components\camera.h"         /*! component::Camera */
#include "..\..\System\Manager\input_manager.h"     /*! InputManager */
#include "..\..\System\Manager\scene_manager.h"     /*! SceneManager */
#include "..\..\System\Shader\shader_wrapper.h"     /*! ShaderWrapper */

ObjectCollidable::ObjectCollidable(const std::string& sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {
    SetScaleValue(16.f);

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

void ObjectCollidable::Update() {
    auto& input = InputManager::GetInstance();
    if (input.)
    input.GetKeyValue();
}

void ObjectCollidable::Draw() {
    auto M = GetModelMatrix();
    auto PV = SceneManager::GetInstance().GetPresentScene()->GetMainCamera()->GetPV();
    auto PVM = PV * M;

    auto& shader = m_sprite_renderer.GetWrapper();
    shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
    shader.ReplaceUniformValue("alpha", 1.0f);
    m_sprite_renderer.RenderSprite();
}

