
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Object/rotatable_obstacle.h
///

/// Header file
#include "../../Include/Object/rotatable_obstacle.h"

#include <Manager/scene_manager.h>

/// ::opgs16::component::CCamera
#include <Component/camera.h>
/// ::opgs16::component::CSprite2DRenderer
#include <Component/sprite_renderer.h>
/// ::opgs16::component::CRigidbody2D
#include <Component/rigidbody_2d.h>
/// ::opgs16::physics::CRectangleCollider2D
#include <Component/Physics2D/Collider/rectangle.h>

namespace debug::object {

RotatableObject::RotatableObject() {
  SetScaleValue(32.0f);
  SetWorldPosition({128, 112, 0});

  using opgs16::component::CSprite2DRenderer;
  using opgs16::component::CRigidbody2D;

  auto renderer = AddComponent<CSprite2DRenderer>(*this, "System", "gQuad");
  renderer->SetTextureIndex(8);

  m_wrapper = &renderer->Wrapper();
  m_wrapper->SetUniformValue<glm::mat4>("projection", glm::mat4{});
  m_wrapper->SetUniformValue<float>("alpha", 1.0f);

  using opgs16::physics::CRectangleCollider2D;
  auto rigidbody2d = AddComponent<CRigidbody2D>(*this);
  rigidbody2d->AddCollider2D<CRectangleCollider2D>(-32.f, 32.f, 32.f, -32.f);
}

void RotatableObject::Render() {
  using opgs16::manager::MSceneManager;
  using opgs16::component::CSprite2DRenderer;

  m_wrapper->SetUniformValue<glm::mat4>("projection",
      MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix());
  GetComponent<CSprite2DRenderer>()->RenderSprite();
}

} /// ::debug::object namespace