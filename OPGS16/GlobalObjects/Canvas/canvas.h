#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H

/**
 * @file GlobalObjects\Canvas\image.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @date 2018-02-19
 *
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
 */

#include "..\..\System\Object\ui_object.h"  /*! UiObject */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! glm::mat4
                                              * helper::ShaderNew;
                                              * camera::CameraObject; */

namespace canvas {

/**
 * @class Canvas
 * @brief Canvas frame object saves and manages UI components and permit accessing from UI m_object_list.
 *
 * This canvas offers several features. (and comming soon features)
 * 1. Each update frame, canvas updates orthographic projection size
 * 2. Each draw frame, canvas object calls UI components.
 * 3. UI components can access Canvas object (parent), get orthographic projection.
 * 4. when Canvas object is destroyed, children UI components are automatically disorganized.
 *
 * @date 2018-02-19
 *
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
 */
class Canvas : public UiObject {
public:
	/**
	 * @brief Canvas constructor.
	 * Canvas has orthographic camera fixed to screen size, this camera will be initialized.
	 */
	Canvas();
	virtual ~Canvas() = default;

	virtual void LocalUpdate() override;

    /*! Disable DEPTH_TEST until All UiObject rendering is done. */
	virtual void Render() override;

	/** Get Ui camera's orthographic projection matrix. */
	const glm::mat4 GetUiCameraPVMatrix() const;

private:
	/** Camera object uses just one orthographic type. */
    opgs16::component::Camera* m_camera{ nullptr };

	bool m_is_size_changed;
};

}

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H */