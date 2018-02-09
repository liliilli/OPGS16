#ifndef OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H
#define OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H

/**
 * @file GlobalObjects/camera_object.h
 * @brief This file consists of Camera object, not Legacy Camera.
 * Last Updated :: 2018 - 02 - 02
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */
#include "..\System\Object\object.h"
#include <glm\glm.hpp>

namespace camera {

/**
 * @class CameraObject
 * @brief Refined camera object class, different legacy camera object is independent of objects.
 * You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
 * If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
 * no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
 */
class CameraObject final : public Object {
public:
	/** Camera type how to see worlds. */
	enum class ViewType {
		PERSPECTIVE, /** This camera uses perspective camera, using view-frustum. */
		ORTHO		 /** This camera uses orthographic camera, using view-cube. */
	};

	/** Camera priority type. */
	enum class CameraType {
		MAIN,	/** Main camera, this must just be only one in one scene. (even hierarchy) */
		SUB,	/** Sub camera, this type permits one more cameras in one scene. */
	};

	CameraObject(ViewType view_type, CameraType camera_type, bool _auto = true);
	virtual ~CameraObject();

	/** Get Projection matrix */
	glm::mat4 GetProjectionMatrix();

private:
	mutable ViewType m_viewtype;	/** View type variable */
	mutable CameraType m_cameratype;/** Camera type variable */

	glm::mat4 m_projection{};		/** Projection matrix world space or ui canvas to screen. */
	float m_near, m_far;			/** Distance sets region of sight. used only for perspective */
	float m_perspective_angle;		/** Angle sets how to see world, used only for perspective */

	static bool s_main_camera_initiated;	/** Check flag if main camera is already initiated */

private:
	/** Initiate orthographic projection. automatically set it to screen size. */
	[[noreturn]] void InitiateOrthographicProjection();

	/** Initiate perspective projection matrix. automatically set it to screen size ratio. */
	[[noreturn]] void InitiatePerspectiveProjection();
};

}

#endif /** OPGL16_GLOBAL_OBJECTS_CAMERA_OBJECT_H */

