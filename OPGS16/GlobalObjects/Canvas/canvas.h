#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @date 2018-02-08
 */

/** UiObject */
#include "..\..\System\Object\ui_object.h"
/**
 * glm::mat4
 * helper::ShaderNew;
 * camera::CameraObject; */
#include "..\..\Headers\Fwd\objectfwd.h"

namespace Canvas {

/**
 * @class Canvas
 * @brief Canvas frame object saves and manages UI components and permit accessing from UI objects.
 *
 * This canvas offers several features. (and comming soon features)
 * 1. Each update frame, canvas updates orthographic projection size
 * 2. Each draw frame, canvas object calls UI components.
 * 3. UI components can access Canvas object (parent), get orthographic projection.
 * 4. when Canvas object is destroyed, children UI components are automatically disorganized.
 */
class Canvas : public UiObject {
public:
	/**
	 * @brief Canvas constructor.
	 * Canvas has orthographic camera fixed to screen size, this camera will be initialized.
	 */
	Canvas();
	virtual ~Canvas() = default;

	[[noreturn]] virtual void Update() override;

	[[noreturn]] virtual void Draw(helper::ShaderNew& shader) override final {};

	[[noreturn]] virtual void Draw() override;

	/** Get Ui camera's orthographic projection matrix. */
	const glm::mat4 GetUiCameraPVMatrix() const;

private:
	/** Camera object uses just one orthographic type. */
    component::Camera* m_camera{ nullptr };

	bool m_is_size_changed;
};

}

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H */