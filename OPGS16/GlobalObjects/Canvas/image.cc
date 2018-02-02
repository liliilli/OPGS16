#include "image.h"

#include "..\..\System\Frame\constant.h"
#include "..\..\System\Shader\shader_manager.h"

constexpr bool FAILED{ false };
constexpr bool SUCCESS{ true };

namespace Canvas {
Image::Image(const std::string&& image_path, const std::shared_ptr<Canvas>& ref_canvas) :
	texture{ image_path , GL_RGBA },
	quad{ helper::CreateBindingObjectEBO(quad_info, 8,
										 {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}},
										 quad_indices)},
	m_ref_canvas{ ref_canvas } {

	InitiateShader();
}

void Image::Update() {
	/** Update my xywh */
	const auto wh = GetScaleFactor() * GetScaleValue() * 2.f;
	const auto xy = GetFinalPosition() - (wh / 2.0f);

	std::array<GLint, 4>&& xywh = {
		static_cast<GLint>(xy.x), static_cast<GLint>(xy.y),
		static_cast<GLint>(wh.x), static_cast<GLint>(wh.y) };
	UpdateScreenXYWH(xywh);

	/** Update children */
	UiObject::Update();
}

void Image::SetImageSize(const float width, const float height) {
	SetScaleValue(1.0f);
	SetScaleFactor({ width / 2.0f, height / 2.0f, 0 });
}

void Image::InitiateShader() {
	auto& manager = ShaderManager::GetInstance();
	shader = manager.GetShaderWithName("gQuad");
	if (!shader) {
		using Type = helper::ShaderNew::Type;
		using namespace std::string_literals;

		shader = manager.CreateShader("gQuad", {
			{Type::VS, "Shaders/Global/image.vert"s},
			{Type::FS, "Shaders/Global/image.frag"s}
			});
	}
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
	shader->Use();

	auto PVM = GetPvmMatrix();
	shader->SetVecMatrix4f("projection", PVM);
	shader->SetFloat("alpha", alpha);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetId());

	glBindVertexArray(quad.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

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
	auto P = m_ref_canvas.lock()->GetUiCameraProjMatrix();

	return P * V * M;
}
}