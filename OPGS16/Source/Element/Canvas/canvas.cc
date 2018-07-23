#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Canvas/canvas.cc
///
/// @brief Definition file of ./canvas.h
///
/// @log
/// 2018-04-17 Add comments and move definition functions to ::opgs16::element::canvas namespace.
/// 2018-04-17 Move file path to System/Element/Canvas/Private.
///

/// Header file
#include <Element/Canvas/canvas.h>

#include <array>
/// ::opgs16::component::CCamera
#include <Component/camera.h>
#include <Core/core_setting.h>

namespace opgs16::element::canvas {

CCanvas::CCanvas() : m_is_size_changed{ true } {
  using opgs16::component::CCamera;
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;
  using EViewType = opgs16::component::CCamera::ViewType;
  using ECamType = opgs16::component::CCamera::CameraType;

  m_camera = AddComponent<CCamera>(*this, EViewType::ORTHO, ECamType::SUB);

  const int32_t half_width   = GetScreenWidth() / 2;
  const int32_t half_height  = GetScreenHeight() / 2;
  m_camera->SetWorldPosition({half_width, half_height, 10});
}

void CCanvas::LocalUpdate() {
  using opgs16::setting::GetScreenWidth;
  using opgs16::setting::GetScreenHeight;

	if (m_is_size_changed) {
		UpdateScreenXYWH({0, 0, GetScreenWidth(), GetScreenHeight()});
		m_is_size_changed = false;
	}

	UiObject::LocalUpdate();
}

void CCanvas::Render() {
  glDisable(GL_DEPTH_TEST);

  for (auto& child : GetGameObjectList())
    child.second->Draw();

  glEnable(GL_DEPTH_TEST);
}

const glm::mat4& CCanvas::GetUiCameraPVMatrix() const {
	return m_camera->PvMatrix();
}

const glm::mat4& CCanvas::GetUiCameraProjectMatrix() const noexcept {
  return m_camera->ProjectionMatrix();
}

const glm::mat4& CCanvas::GetUiCameraViewMatrix() const noexcept {
  return m_camera->ViewMatrix();
}

} /// ::opgs16::element::canvas namespace
