
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Canvas/image.cc
///
/// @brief Definition file of ./image.h
///
/// @log
/// 2018-04-17 Add comments and move definition functions to ::opgs16::element::canvas namespace.
/// 2018-04-17 Rearrange file path of #include macros.
///

/// Header file
#include <Element/Canvas/image.h>

#include <array>
#include <glm/gtc/matrix_transform.hpp>

/// ::opgs16::component::CSprite2DRenderer
#include <Component/sprite2d_renderer.h>
#include <Element/Canvas/canvas.h>
#include <Manager/Internal/shader_builtin_keywords.h>
#include <Shader/shader_wrapper.h>
#include <Shader/Default/shader_quad2d.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::element::canvas {

CImage::CImage(const std::string& sprite_tag,
               const CCanvas* const ref_canvas) :
    m_ref_canvas{ const_cast<CCanvas*>(ref_canvas) } {
  m_renderer_ptr = AddComponent<component::CSprite2DRenderer>(
      *this, sprite_tag, "opQuad2d");
}

CImage::CImage(const std::string& sprite_tag,
               const std::unique_ptr<CCanvas>& ref_canvas) :
    CImage{ sprite_tag, ref_canvas.get() } {
}

void CImage::SetImageSize(const float width, const float height) {
	SetScaleValue(1.0f);
	SetScaleFactor({ width / 2.0f, height / 2.0f, 0 });
}

void CImage::SetTextureFragmentIndex(const int32_t index) {
  PHITOS_ASSERT(index >= 0, "Index must be positive integer.");
  m_renderer_ptr->SetTextureFragmentIndex(index);
}

void CImage::LocalUpdate() {
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

  UiObject::LocalUpdate();
}

void CImage::Render() {
	auto& wrapper = m_renderer_ptr->GetWrapper();
	wrapper.SetUniformValue<glm::mat4>(builtin::s_uniform_proj,
                                     m_ref_canvas->GetUiCameraProjectMatrix());
	wrapper.SetUniformValue<glm::mat4>(builtin::s_uniform_view,
                                     m_ref_canvas->GetUiCameraViewMatrix());
	wrapper.SetUniformValue<glm::mat4>(builtin::s_uniform_model,
                                     GetModelMatrix());
	wrapper.SetUniformValue(builtin::s_uniform_alpha, 1.0f);

	m_renderer_ptr->RenderSprite();
}

void CImage::SetRenderingLayer(const std::string& layer_name) {
  m_renderer_ptr->SetRenderingLayer(layer_name);
}

void CImage::SetRenderingLayer(int32_t layer_value) {
  m_renderer_ptr->SetRenderingLayer(layer_value);
}

} /// ::opgs16::element::canvas namespace
