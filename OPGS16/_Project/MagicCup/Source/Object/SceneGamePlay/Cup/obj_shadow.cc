
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include "../../../../Include/Object/SceneGamePlay/Cup/obj_shadow.h"

#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

ObjectCupShadow::ObjectCupShadow() {
  SetScaleValue(16.f);
  SetScaleFactor({2.f, 1.f, 0.f});

  m_texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite, "opQuad2d", 10);
  m_wrapper = &m_texture->GetWrapper();

  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;
  using namespace opgs16::builtin;
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_proj,
                                        GetPresentMainCamProjMatrix());
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_view,
                                        GetPresentMainCamViewMatrix());
  m_wrapper->SetUniformValue<float>(s_uniform_alpha, 0.6f);
}

void ObjectCupShadow::Render() {
  using namespace opgs16::builtin;
  m_wrapper->SetUniformValue<glm::mat4>(s_uniform_model, GetModelMatrix());
  m_texture->RenderSprite();
}

} /// ::magiccup namespace