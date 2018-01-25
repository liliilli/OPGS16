#include "image.h"

#include "..\..\System\Frame\constant.h"
#include "..\..\System\Shader\shader_manager.h"

constexpr bool FAILED{ false };
constexpr bool SUCCESS{ true };

namespace Canvas {
	Image::Image(std::string&& image_path) :
		texture{ image_path , GL_RGB },
		quad{ helper::CreateBindingObject(quad_info, 8, {{0, 3, 0}, {1, 3, 3}, {2, 2, 6}}) } {

		InitiateShader();
	}

	void Image::InitiateShader() {
		auto& manager = ShaderManager::GetInstance();
		shader = manager.GetShaderWithName("gQuad");
		if (!shader) {
			using Type = helper::ShaderNew::Type;
			using namespace std::string_literals;

			shader = manager.CreateShader("gQuad", {
				{Type::VS, "Shaders/Bloom/normal.vert"s},
				{Type::FS, "Shaders/Bloom/normal.frag"s}
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
		GetModelMatrix();
		shader->SetVecMatrix4f("uModel", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetId());

		glBindVertexArray(quad.vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (!is_already_enabled) glDisable(GL_BLEND);

		/** Render Children */
		for (const auto& child : children) {

		}
	}

	bool Image::DestroyChild(std::string& tag) {
		decltype(children.end()) iterator = children.find(tag);
		if (iterator == children.end())
			return FAILED;

		children.erase(tag);
		return SUCCESS;
	}

	const std::vector<std::string> Image::GetChildrenTags() const {
		std::vector<std::string> list(children.size());
		for (const auto& object_pair : children) {
			list.push_back(object_pair.first);
		}
		return list;
	}
}