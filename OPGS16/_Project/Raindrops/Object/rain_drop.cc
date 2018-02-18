#include "rain_drop.h"

#include <random>

#include "..\Scripts\drops.h"   /*! script::Drops */
#include "..\..\..\System\Components\camera.h"         /*! component::Camera */
#include "..\..\..\System\Manager\object_manager.h"    /*! ObjectManager */
#include "..\..\..\System\Manager\scene_manager.h"     /*! SceneManager */
#include "..\..\..\System\Shader\shader_wrapper.h"     /*! ShaderWrapper */
#include "..\..\..\System\Components\Physics2D\rigidbody_2d.h"

RainDrop::RainDrop(const std::string& sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {
    SetScaleValue(5.f);
    SetScaleFactor({ 0.1, 1, 0 });

    AddComponent<Drops>(*this);
    AddComponent<component::Rigidbody2D>(*this);
    component::Rigidbody2D* const rigidbody = GetComponent<component::Rigidbody2D>();
    rigidbody->AddCollider2D<collision::RectangleCollider2D>(-1.f, 5.f, 1.f, -5.f);

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

void RainDrop::Draw() {
    auto i = GetWorldPosition();
    if (-2 <= i.x && i.x <= 256 && i.y <= 224 && -6 <= i.y) {
        auto M = GetModelMatrix();
        auto PV = SceneManager::GetInstance().GetPresentScene()->GetMainCamera()->GetPV();
        auto PVM = PV * M;

        auto& shader = m_sprite_renderer.GetWrapper();
        shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
        shader.ReplaceUniformValue("alpha", 1.0f);
        m_sprite_renderer.RenderSprite();
    }
}

void RainDrop::OnCollisionEnter(component::Rigidbody2D& body) {
    if (body.IsTag(2)) {
        std::random_device rng;
        auto x = rng() % 256;
        auto y = rng() % 228 + 228;

        SetLocalPosition({ x, y, 0 });
    }
}
