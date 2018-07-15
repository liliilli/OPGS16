/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file GlobalObjects/Canvas/image.cc
 * @brief Definition file of ./image.h
 * @log
 * 2018-04-17 Add comments and move definition functions to ::opgs16::element::canvas namespace.
 * 2018-04-17 Rearrange file path of #include macros.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element/Canvas/image.h>       /// Header file

#include <array>                        /// std::array
#include <glm/gtc/matrix_transform.hpp>

/// ::opgs16::component::CSprite2DRenderer
#include <Component/sprite2d_renderer.h>
#include <Element/Canvas/canvas.h>      /// ::opgs16::element::canvas::CCanvas
#include <Manager/texture_manager.h>    /// ::opgs16::manager::MTextureManager
#include <Shader/shader_wrapper.h>      /// ::opgs16::element::CShaderWrapper
#include <Shader/Default/shader_quad2d.h>
#include <Phitos/Dbg/assert.h>

namespace opgs16::element::canvas {

CImage::CImage(const std::string& sprite_tag,
               const CCanvas* const ref_canvas) :
    m_ref_canvas{ const_cast<CCanvas*>(ref_canvas) } {
  m_renderer_ptr = AddComponent<component::CSprite2DRenderer>(
      *this, sprite_tag, builtin::shader::SGlobalQuad2D::s_shader_name);
}

CImage::CImage(const std::string& sprite_tag,
               const std::unique_ptr<CCanvas>& ref_canvas) :
    CImage{ sprite_tag, ref_canvas.get() } {
}

void CImage::SetImageSize(const float width, const float height) {
	SetScaleValue(1.0f);
	SetScaleFactor({ width / 2.0f, height / 2.0f, 0 });
}

void CImage::SetTextureIndex(const int32_t index) {
  PHITOS_ASSERT(index >= 0, "Index must be positive integer.");
  m_renderer_ptr->SetTextureFragmentIndex(index);
}

void CImage::LocalUpdate() {
  // Update xywh
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

  UiObject::LocalUpdate();
}

void CImage::Render() {
	auto is_already_enabled{ false };
	if (glIsEnabled(GL_BLEND)) is_already_enabled = true;
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	auto& wrapper = m_renderer_ptr->GetWrapper();
	wrapper.SetUniformValue<glm::mat4>("opProj", m_ref_canvas->GetUiCameraPVMatrix());
	wrapper.SetUniformValue<glm::mat4>("opModel", GetModelMatrix());
	wrapper.SetUniformValue("opAlpha", 1.0f);

	m_renderer_ptr->RenderSprite();

	if (!is_already_enabled) {
    glDisable(GL_BLEND);
	}
}

} /// ::opgs16::element::canvas namespace
