
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"

#include <Component/sprite_renderer.h>
#include <Manager/scene_manager.h>

#include "../../../../Include/Internal/general_keyword.h"
#include "../../../../Include/Script/SceneGamePlay/Cup/script_cup_number.h"

namespace magiccup {

ObjectCup::ObjectCup() {
  SetScaleValue(16.f);
  SetScaleFactor({2.f, 3.f, 0.f});

  auto cup_texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite, "gQuad", 4);
  m_wrapper = &cup_texture->Wrapper();
  m_cup_texture = cup_texture;

  m_numbering_script = AddComponent<ScriptCupNumbering>(*this);
}

void ObjectCup::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());
  m_cup_texture->RenderSprite();
}

} /// ::magiccup namespace