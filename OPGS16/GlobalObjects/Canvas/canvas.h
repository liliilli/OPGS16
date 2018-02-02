#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_CANVAS_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_CANVAS_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <array>
#include "..\..\GlobalObjects\camera_object.h"
#include "..\..\Headers\common.h"

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
	Canvas();
	virtual ~Canvas() = default;

	[[noreturn]] virtual void Update() override final;

	[[noreturn]] virtual void Draw(helper::ShaderNew& shader) override final {};

	[[noreturn]] virtual void Draw() override final;

	/** Get Ui camera's orthographic projection matrix. */
	glm::mat4 GetUiCameraProjMatrix();

private:
	camera::CameraObject m_camera;	/** Camera object uses just one orthographic type. */
	bool m_is_size_changed{ true };
};

}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_CANVAS_H */