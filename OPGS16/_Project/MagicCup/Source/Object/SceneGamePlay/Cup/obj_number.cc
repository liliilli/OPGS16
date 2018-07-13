
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../../Include/Object/SceneGamePlay/Cup/obj_number.h"

#include <Component/sprite_renderer.h>
#include <Manager/scene_manager.h>

#include "../../../../Include/Internal/general_keyword.h"

namespace magiccup {

ObjectNumber::ObjectNumber() {
  SetScaleValue(8.f);
  auto texture = AddComponent<opgs16::component::CSprite2DRenderer>(
      *this, keyword::rsc_sprite2, "gQuad", 42);
  m_wrapper = &texture->Wrapper();
  m_texture = texture;
}

void ObjectNumber::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());
  m_texture->RenderSprite();
}

} /// ::magiccup namespace