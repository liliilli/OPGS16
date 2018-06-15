
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
/// ::opgs16::component::CRigidbody2DCompact
#include <Component/rigidbody2d_compact.h>

#include <Helper/vector.h>

namespace debug::object {

RotatableObject::RotatableObject() {
  SetScaleValue(32.0f);
  SetWorldPosition({128, 112, 0});

  using opgs16::component::CSprite2DRenderer;
  auto renderer = AddComponent<CSprite2DRenderer>(*this, "System", "gQuad");
  renderer->SetTextureIndex(8);
  m_wrapper = &renderer->Wrapper();
  m_wrapper->SetUniformValue<glm::mat4>("projection", glm::mat4{});
  m_wrapper->SetUniformValue<float>("alpha", 1.0f);

  using opgs16::component::CRigidbody2DCompact;
  auto rigidbody = AddComponent<CRigidbody2DCompact>(*this);
  rigidbody->AddCollider2D<opgs16::EColliderType::Box2D>(
      opgs16::DVector3{32, 32, 0});
}

void RotatableObject::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  using opgs16::component::CSprite2DRenderer;

  m_wrapper->SetUniformValue<glm::mat4>("projection",
    GetPresentScenePvMatrix() * GetModelMatrix());
  GetComponent<CSprite2DRenderer>()->RenderSprite();
}

} /// ::debug::object namespace