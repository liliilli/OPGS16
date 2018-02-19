#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Image object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @date 2018-02-19
 *
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method.
 *            Replace Draw(), Update() with Render() and LocalUpdate()
 */

#include "..\..\System\Object\ui_object.h"          /** UiObject */
#include "..\..\System\Object\sprite_renderer.h"    /** SpriteRenderer */
#include "..\..\Headers\Fwd\objectfwd.h"            /** Canvas::Canvas */

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
 *
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method.
 */
class Image : public UiObject {
public:
	Image(const std::string& sprite_tag, const Canvas* const ref_canvas);
	Image(const std::string& sprite_tag, const std::unique_ptr<Canvas>& ref_canvas);

	virtual ~Image() = default;

	/**
	 * @brief Image instance updates x, y, w, h for each frame, referencing with Scale values.
	 * And, updates children calling Parent's Update method.
	 *
	 * This virtual methods can not override any more.
	 */
	 virtual void LocalUpdate() override final;

	/**
	 * @brief This calls callee to draw or render something it has.
	 * Afterward, it draws children calling Parent's Draw (no parameter) method.
	 *
	 * This virtual methods can not override any more.
	 */
	 virtual void Render() override final;

	/**
	 * @brief Set size.
	 */
	 void SetImageSize(const float width, const float height);

private:
	SpriteRenderer m_sprite_renderer;	/** Sprite Renderer */
	Canvas* const m_ref_canvas;		/** l_value reference of canvas to get projection matrix. */

};
}

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H */
