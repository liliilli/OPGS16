#include "random_moving.h"
#include "..\..\System\Components\camera.h"         /*! component::Camera */
#include "..\..\System\Components\Physics2D\rigidbody_2d.h" /*! component::Rigidbody2D */
#include "..\..\System\Components\Physics2D\Collider\rectangle.h"   /*! collision::Rectangle*/
#include "..\..\System\Manager\scene_manager.h"     /*! SceneManager */
#include "..\..\System\Shader\shader_wrapper.h"     /*! ShaderWrapper */

#include "..\..\System\Manager\object_manager.h"    /*! ObjectManager */

#include <iostream>

RandomMoveObject::RandomMoveObject(const std::string & sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {
    SetScaleValue(8.f);
    AddComponent<component::Rigidbody2D>(*this);
    component::Rigidbody2D* const rigidbody = GetComponent<component::Rigidbody2D>();
    rigidbody->AddCollider2D<collision::RectangleCollider2D>(-8.f, 8.f, 8.f, -8.f);

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

void RandomMoveObject::Update() {
    Object::Update();
}

void RandomMoveObject::Draw() {
    auto M = GetModelMatrix();
    auto PV = SceneManager::GetInstance().GetPresentScene()->GetMainCamera()->GetPV();
    auto PVM = PV * M;

    auto& shader = m_sprite_renderer.GetWrapper();
    shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
    shader.ReplaceUniformValue("alpha", 1.0f);
    m_sprite_renderer.RenderSprite();
}

void RandomMoveObject::OnCollisionEnter(component::Rigidbody2D & collider) {
    if (collider.IsTag("Player")) {
        ObjectManager::GetInstance().Destroy(*this);
    }
}
