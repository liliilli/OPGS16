
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Debug/Object/hopping.cc
///

/// Header file
#include "../../Include/Object/hopping.h"

/// ::opgs16::component::CCamera
#include <Component/camera.h>
/// ::opgs16::component::CSprite2DRenderer
#include <Component/sprite_renderer.h>
/// ::opgs16::manager::MSceneManager
#include <Manager/scene_manager.h>
#include <Shader/shader_wrapper.h>

#include "../../Include/Script/hopping.h"

namespace debug::object {

Hopping::Hopping() {
  SetScaleValue(16.0f);

  using opgs16::component::CSprite2DRenderer;
  auto renderer = AddComponent<CSprite2DRenderer>(*this, "System", "gQuad");

  m_wrapper = &renderer->Wrapper();
  m_wrapper->SetUniformValue<glm::mat4>("projection", glm::mat4{});
  m_wrapper->SetUniformValue<float>("alpha", 1.0f);

  AddComponent<script::Hopping>(*this);
}

void Hopping::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  auto PVM = GetPresentScenePvMatrix() * GetModelMatrix();

  m_wrapper->SetUniformValue<glm::mat4>("projection", PVM);
  using opgs16::component::CSprite2DRenderer;
  GetComponent<CSprite2DRenderer>()->RenderSprite();
}

} /// ::debug::object
