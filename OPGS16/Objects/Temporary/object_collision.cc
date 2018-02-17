#include "object_collision.h"
#include "..\..\System\Components\camera.h"         /*! component::Camera */
#include "..\..\System\Components\Physics2D\rigidbody_2d.h" /*! component::Rigidbody2D */
#include "..\..\System\Components\Physics2D\Collider\rectangle.h"   /*! collision::Rectangle*/
#include "..\..\System\Manager\input_manager.h"     /*! InputManager */
#include "..\..\System\Manager\scene_manager.h"     /*! SceneManager */
#include "..\..\System\Shader\shader_wrapper.h"     /*! ShaderWrapper */

#include <iostream>

ObjectCollidable::ObjectCollidable(const std::string& sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {
    SetScaleValue(16.f);
    AddComponent<component::Rigidbody2D>(*this);
    component::Rigidbody2D* const rigidbody = GetComponent<component::Rigidbody2D>();

    auto i = std::make_unique<collision::RectangleCollider2D>(-16.f, 16.f, 16.f, -16.f);
    rigidbody->AddCollider2D(std::move(i));

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

void ObjectCollidable::Update() {
    auto& input = InputManager::GetInstance();

    auto position = GetLocalPosition();
    position.x += 2 * input.GetKeyValue("Horizontal");
    position.y += 2 * input.GetKeyValue("Vertical");
    SetLocalPosition(position);

    Object::Update();
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

void ObjectCollidable::OnCollisionEnter(component::Rigidbody2D& collider) {
    if (collider.IsTag("Enemy"))
        std::cout << "Hello there\n";
}

