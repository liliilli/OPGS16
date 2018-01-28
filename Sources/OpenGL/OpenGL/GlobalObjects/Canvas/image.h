#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Image object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\..\Headers\common.h"

namespace Canvas {

/**
 * @class Image
 * @brief UI Image to display one patched image.
 *
 * This class are able to have children, but only UiObject. unless, Run-time error will be invoked.
 * Actual display position of children(UiImage) will be constrained by Image's center position and
 * size.
 *
 * Scale binding and Rotation binding has not been implemented yet.
 */
class Image : public UiObject {
public:
	Image(std::string&& image_path);
	virtual ~Image() = default;

	/**
	 * @brief Image instance updates x, y, w, h for each frame, referencing with Scale values.
	 * And, updates children calling Parent's Update method.
	 *
	 * This virtual methods can not override any more.
	 */
	[[noreturn]] virtual void Update() override final;

	/**
	 * @brief The method calls scene to one objects.
	 * @param[in] null NOT USE.
	 * This virtual methods can not override any more.
	 */
	[[noreturn]] virtual void Draw(helper::ShaderNew&) override final;

	/**
	 * @brief This calls callee to draw or render something it has.
	 * Afterward, it draws children calling Parent's Draw (no parameter) method.
	 *
	 * This virtual methods can not override any more.
	 */
	[[noreturn]] virtual void Draw() override final;

	/**
	 * @brief Set size.
	 */
	[[noreturn]] void SetImageSize(const float width, const float height);

private:
	void InitiateShader();

	glm::mat4 GetPvmMatrix();

private:
	helper::BindingObject quad;
	texture::Texture2D texture;
	std::shared_ptr<helper::ShaderNew> shader;

	const float alpha{ 1.0f };
};
}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_IMAGE_H */
