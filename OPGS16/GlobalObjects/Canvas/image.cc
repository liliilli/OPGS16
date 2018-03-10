#include "image.h"

#include <array>            /*! std::array */
#include "canvas.h"
#include "..\..\System\Shader\shader_wrapper.h"        /*! ShaderWrapper */
#include "../../System/Manager/Public/texture_manager.h"
#include <glm\gtc\matrix_transform.hpp>

namespace canvas {
Image::Image(const std::string& sprite_tag, const Canvas* const ref_canvas) :
	m_sprite_renderer{ sprite_tag, "gQuad" },
	m_ref_canvas{ const_cast<Canvas*>(ref_canvas) } {

	auto& shader = m_sprite_renderer.Wrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);
}

Image::Image(const std::string& sprite_tag, const std::unique_ptr<Canvas>& ref_canvas) :
	Image{ sprite_tag, ref_canvas.get() } { }

void Image::LocalUpdate() {
	/** Update my xywh */
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	UiObject::LocalUpdate();
}

void Image::SetImageSize(const float width, const float height) {
	SetScaleValue(1.0f);
	SetScaleFactor({ width / 2.0f, height / 2.0f, 0 });
}

void Image::Render() {
	auto is_already_enabled{ false };
	if (glIsEnabled(GL_BLEND)) is_already_enabled = true;
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/** Render this */
	auto& shader = m_sprite_renderer.Wrapper();

	auto PVM = m_ref_canvas->GetUiCameraPVMatrix() * GetModelMatrix();
	shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
	shader.ReplaceUniformValue("alpha", 1.0f);
	m_sprite_renderer.RenderSprite();

	if (!is_already_enabled) glDisable(GL_BLEND);
}

}