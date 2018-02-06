#include "image.h"

#include "..\..\System\Frame\constant.h"
#include "..\..\System\Shader\shader_manager.h"

#include "..\..\System\Manager\texture_manager.h"

constexpr bool FAILED{ false };
constexpr bool SUCCESS{ true };

namespace Canvas {
Image::Image(const std::string& sprite_tag, const Canvas* const ref_canvas) :
	m_sprite_renderer{ sprite_tag, "gQuad" },
	m_ref_canvas{ const_cast<Canvas*>(ref_canvas) } {
}

Image::Image(const std::string& sprite_tag, const std::unique_ptr<Canvas>& ref_canvas) :
	Image{ sprite_tag, ref_canvas.get() } { }

void Image::Update() {
	/** Update my xywh */
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.InsertUniformValue<glm::mat4>("projection", glm::mat4{});
	shader.InsertUniformValue<float>("alpha", 0.0f);

	/** Update children */
	UiObject::Update();
}

void Image::SetImageSize(const float width, const float height) {
	SetScaleValue(1.0f);
	SetScaleFactor({ width / 2.0f, height / 2.0f, 0 });
}

void Image::Draw(helper::ShaderNew&) {
	Draw();
}

void Image::Draw() {
	auto is_already_enabled{ false };
	if (glIsEnabled(GL_BLEND)) is_already_enabled = true;
	else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/** Render this */
	auto PVM = GetPvmMatrix();

	auto& shader = m_sprite_renderer.GetWrapper();
	shader.ReplaceUniformValue<glm::mat4>("projection", PVM);
	shader.ReplaceUniformValue("alpha", 1.0f);
	m_sprite_renderer.RenderSprite();

	if (!is_already_enabled) glDisable(GL_BLEND);

	/** Render Children */
	UiObject::Draw();
}

glm::mat4 Image::GetPvmMatrix() {
	auto M = glm::mat4();
	auto position = GetFinalPosition();
	M = glm::translate(M, glm::vec3{ position.x, position.y , 0 });
	M = glm::scale(M, GetScaleFactor() * GetScaleValue());

	auto V = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	auto P = m_ref_canvas->GetUiCameraProjMatrix();

	return P * V * M;
}
}