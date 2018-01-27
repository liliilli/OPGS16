#include "image.h"

#include "..\..\System\Frame\constant.h"
#include "..\..\System\Shader\shader_manager.h"

constexpr bool FAILED{ false };
constexpr bool SUCCESS{ true };

namespace Canvas {
Image::Image(std::string&& image_path) :
	texture{ image_path , GL_RGBA },
	quad{ helper::CreateBindingObjectEBO(
		quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}, quad_indices)} {

	InitiateShader();
}

void Image::Update() {
	for (auto& child : GetChildren()) {
		child.second->RefreshFinalPosition(GetFinalPosition());
	}
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

void Image::Draw(helper::ShaderNew& shader) {
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
	for (const auto& child : GetChildren()) {
		child.second->Draw();
	}
}

glm::mat4 Image::GetPvmMatrix() {
	auto M = glm::mat4();

	auto position = CalculateCenterPosition(GetOrigin(), glm::vec2{ GetLocalPosition() });

	M = glm::translate(M, glm::vec3{ position.x, position.y , 0 });
	M = glm::scale(M, glm::vec3(width, height, 0));

	auto V = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	auto P = glm::ortho(0.f, 720.f, 0.f, 480.f);

	return P * V * M;
}
}