
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/ObjectTest/node_cursor.h"

#include <Component/sprite_renderer.h>
#include <Manager/scene_manager.h>
#include <Shader/shader_wrapper.h>

#include "../../../Include/Script/ObjectTest/node_script.h"

namespace debug::object {

NodeCursorObject::NodeCursorObject() {
  SetScaleValue(12.0f);

  using opgs16::component::CSprite2DRenderer;
  auto renderer = AddComponent<CSprite2DRenderer>(*this, "opSystem", "gQuad");
  renderer->SetTextureIndex(16);

  m_wrapper = &renderer->Wrapper();
  m_wrapper->SetUniformValue<glm::mat4>("opProj", glm::mat4{});
  m_wrapper->SetUniformValue<glm::mat4>("opModel", glm::mat4{});
  m_wrapper->SetUniformValue<float>("opAlpha", 1.0f);
}

void NodeCursorObject::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());

  using opgs16::component::CSprite2DRenderer;
  GetComponent<CSprite2DRenderer>()->RenderSprite();
}

} /// ::debug::object namespace