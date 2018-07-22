
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/effect_boom_object.h"

#include <Component/animator.h>
#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>
#include <Manager/Internal/shader_builtin_keywords.h>

#include "../../../Include/Internal/general_keyword.h"
#include "../../../Include/Script/SceneGamePlay/Disposable/effect_boom_script.h"

namespace magiccup {

EffectBoomObject::EffectBoomObject() {
  using opgs16::component::CSprite2DRenderer;
  using opgs16::component::CAnimator;

  SetScaleValue(16.f);
  SetScaleFactor({1.f, 1.f, 0.f});

  m_renderer = AddComponent<CSprite2DRenderer>(*this, keyword::rsc_sprite, "opQuad2d");
  m_wrapper  = &m_renderer->GetWrapper();

  m_animator = AddComponent<CAnimator>(*this, *m_renderer, "Boom");
  AddComponent<ScriptEffectBoom>(*this);
}

void EffectBoomObject::Render() {
  using opgs16::manager::scene::GetPresentMainCamProjMatrix;
  using opgs16::manager::scene::GetPresentMainCamViewMatrix;
  using namespace opgs16::builtin;

  m_wrapper->SetUniformMat4(s_uniform_proj, GetPresentMainCamProjMatrix());
  m_wrapper->SetUniformMat4(s_uniform_view, GetPresentMainCamViewMatrix());
  m_wrapper->SetUniformMat4(s_uniform_model, GetModelMatrix());

  m_renderer->RenderSprite();
}

} /// ::magiccup namespace