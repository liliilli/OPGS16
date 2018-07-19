
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_cup.h"

#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

#include "../../../../Include/Internal/general_keyword.h"
#include "../../../../Include/Script/SceneGamePlay/Cup/script_cup_number.h"

namespace magiccup {

ObjectCup::ObjectCup() {
  SetScaleValue(16.f);
  SetScaleFactor({2.f, 3.f, 0.f});
  SetLocalPosition({0.f, -32.f, 0.f});

  m_cup_texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite,
      "opQuad2d", 6);
  m_wrapper = &m_cup_texture->GetWrapper();

  m_numbering_script = AddComponent<ScriptCupNumbering>(*this);
}

void ObjectCup::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;
  using namespace opgs16::builtin;

  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_proj,
                                        GetPresentMainCamProjMatrix());
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_view,
                                        GetPresentMainCamViewMatrix());
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_model, GetModelMatrix());

  m_cup_texture->RenderSprite();
}

} /// ::magiccup namespace