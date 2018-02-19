#include "object_collision.h"       /*! Header file */
#include "..\Scripts\player.h"  /*! Player */
#include "..\..\..\System\Components\camera.h"     /*! component::Camera */
#include "..\..\..\System\Components\Physics2D\rigidbody_2d.h" /*! component::Rigidbody2D */
#include "..\..\..\System\Components\Physics2D\Collider\rectangle.h"   /*! collision::Rectangle*/
#include "..\..\..\System\Manager\input_manager.h" /*! InputManager */
#include "..\..\..\System\Manager\scene_manager.h" /*! SceneManager */
#include "..\..\..\System\Shader\shader_wrapper.h" /*! ShaderWrapper */


ObjectCollidable::ObjectCollidable(const std::string& sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {
    SetScaleValue(8.f);
    AddComponent<Player>(*this);
    AddComponent<component::Rigidbody2D>(*this);
    component::Rigidbody2D* const rigidbody = GetComponent<component::Rigidbody2D>();

    auto i = std::make_unique<collision::RectangleCollider2D>(-8.f, 8.f, 8.f, -8.f);
    rigidbody->AddCollider2D(std::move(i));

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

void ObjectCollidable::Render() {
    auto M = GetModelMatrix();
    auto PV = SceneManager::GetInstance().GetPresentScene()->GetMainCamera()->GetPV();
    auto PVM = PV * M;

    auto& shader = m_sprite_renderer.GetWrapper();
    shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
    shader.ReplaceUniformValue("alpha", 1.0f);
    m_sprite_renderer.RenderSprite();
}

void ObjectCollidable::OnCollisionEnter(component::Rigidbody2D& collider) {}

