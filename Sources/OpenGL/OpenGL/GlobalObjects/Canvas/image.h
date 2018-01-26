#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Image object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <memory>
#include <type_traits>
#include "..\..\Headers\common.h"

namespace Canvas {
class Image : public Object {
public:
	explicit Image(std::string&& image_path);
	virtual ~Image() = default;

	/**
	 * @brief The method updates components of object.
	 */
	[[noreturn]] virtual void Update() override final {};

	/**
	 * @brief The method calls scene to one objects.
	 * @param[in] shader Shader to use.
	 */
	[[noreturn]] virtual void Draw(helper::ShaderNew& shader) override final;

	/**
	 * @brief This calls callee to draw or render something it has.
	 */
	[[noreturn]] virtual void Draw() override final;

private:
	void InitiateShader();

	glm::mat4 GetPvmMatrix();

private:
	helper::BindingObject quad;
	texture::Texture2D texture;
	std::shared_ptr<helper::ShaderNew> shader;

	std::unordered_map<std::string, std::shared_ptr<Object>> children;

	const float alpha{ 1.0f };
	const float width{ 128.f }, height{ 128.f };
};
}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H */
