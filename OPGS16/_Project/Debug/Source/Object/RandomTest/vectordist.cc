
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Object/RandomTest/vectordist.h"

#include <Component/procedural2d_renderer.h>
#include <Manager/scene_manager.h>

#include "../../../Include/Script/RandomTest/vectordist.h"

namespace debug::object {

VectorDistTestObject::VectorDistTestObject() {
  auto renderer = AddComponent<opgs16::component::CProcedural2DRenderer>(*this, "gQuad");
  m_wrapper = renderer->GetWrapper();
  m_wrapper->SetUniformValue<glm::mat4>("opProj", glm::mat4{});
  m_wrapper->SetUniformValue<glm::mat4>("opModel", glm::mat4{});

  AddComponent<script::VectorDistribution>(*this);
}

void VectorDistTestObject::Render() {
  using opgs16::manager::scene::GetPresentScenePvMatrix;
  m_wrapper->SetUniformValue<glm::mat4>("opProj", GetPresentScenePvMatrix());
  m_wrapper->SetUniformValue<glm::mat4>("opModel", GetModelMatrix());

  using opgs16::component::CProcedural2DRenderer;
  GetComponent<CProcedural2DRenderer>()->RenderSprite();
}

} /// ::debug::object namespace