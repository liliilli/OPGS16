
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_number.h"

#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

ObjectNumber::ObjectNumber() {
  SetScaleValue(8.f);
  auto texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite2, "opQuad2d", 42);
  m_wrapper = &texture->GetWrapper();
  m_texture = texture;
}

void ObjectNumber::Render() {
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