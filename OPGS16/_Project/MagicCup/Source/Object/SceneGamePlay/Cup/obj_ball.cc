
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_ball.h"

#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

ObjectBall::ObjectBall() {
  SetScaleValue(16.f);
  SetScaleFactor({1.f, 1.f, 0.f});

  auto cup_texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite, "opQuad2d", 7);
  m_wrapper = &cup_texture->GetWrapper();
  m_texture = cup_texture;
  m_texture->SetRenderingLayer(1);

  this->SetObjectActive(false);
}

void ObjectBall::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;
  using namespace opgs16::builtin;

  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_proj,
                                        GetPresentMainCamProjMatrix());
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_view,
                                        GetPresentMainCamViewMatrix());
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_model, GetModelMatrix());
  m_texture->RenderSprite();
}

} /// ::magiccup namespace