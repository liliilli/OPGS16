
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/SceneGamePlay/object_cursor.h"

#include <Component/sprite2d_renderer.h>
#include <Manager/scene_manager.h>

namespace magiccup {

ObjectCursor::ObjectCursor() {
  SetScaleValue(8.f);
  SetLocalPosition({0.f, -8.f, 0.f});
  SetRotationLocalAngle(opgs16::element::_internal::EDirection::Z, 90.f);

  auto texture = AddComponent<opgs16::component::CSprite2DRenderer>(*this,
      "opSystem", "gQuad", 8, 3);
  m_wrapper = &texture->GetWrapper();
  m_texture = texture;
}

void ObjectCursor::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  //m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());
  m_texture->RenderSprite();
}

} /// ::magiccup namespace