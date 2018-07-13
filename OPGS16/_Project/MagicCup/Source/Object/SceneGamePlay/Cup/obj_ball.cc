
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_ball.h"

#include <Component/sprite_renderer.h>
#include <Manager/scene_manager.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

ObjectBall::ObjectBall() {
  SetScaleValue(16.f);
  SetScaleFactor({1.f, 1.f, 0.f});

  auto cup_texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite, "gQuad", 5);
  m_wrapper = &cup_texture->Wrapper();
  m_texture = cup_texture;
}

void ObjectBall::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());
  m_texture->RenderSprite();
}

} /// ::magiccup namespace