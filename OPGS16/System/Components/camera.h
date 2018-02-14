#ifndef OPGL16_SYSTEM_COMPONENTS_CAMERA_H
#define OPGL16_SYSTEM_COMPONENTS_CAMERA_H

/**
 * @file System\Components\camera_object.h
 * @brief Camera component file.
 * @author Jongmin Yun
 * @date 2018-02-14
 */

#include <glm\glm.hpp>
#include "_macro.h"                         /*! SET_UP_HASH_MEMBER_DERIVED() */
#include "component.h"                      /*! component::Component */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! Object as reference */

/*!
 * @namespace component
 * @brief The namespace stores informations of basic OPGS16 components to be used in each object.
 * Derived user-defined component class should not exist or insert in component namespace.
 */
namespace component {

/**
 * @class CameraObject
 * @brief Refined camera object class, different legacy camera object is independent of objects.
 * You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
 *
 * If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
 * no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
 * @date 2018-02-14
 */
class Camera final : public component::Component {
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

public:
	Camera(Object& bound_obj, ViewType view_type, CameraType camera_type, bool _auto = true);
	virtual ~Camera();

    const glm::mat4 GetViewMatrix() const noexcept;         /*! Get View matrix */
	const glm::mat4 GetProjectionMatrix() const noexcept;   /*! Get Projection matrix */
    const glm::mat4 GetPV() const noexcept;                 /*! Get PV matrix */

private:
	mutable ViewType m_viewtype;	/** View type variable */
	mutable CameraType m_cameratype;/** Camera type variable */

    glm::mat4 m_view{};             /*! View matrix */
	glm::mat4 m_projection{};		/** Projection matrix world space or ui canvas to screen. */
    glm::mat4 m_PV{};               /*! Projection * View matrix */

	float m_near, m_far;			/** Distance sets region of sight. used only for perspective */
	float m_perspective_angle;		/** Angle sets how to see world, used only for perspective */

    Object& m_bound_object;             /*! Bound object which script instance refers to */
    static bool s_main_camera_initiated;/** Check flag if main camera is already initiated */

private:
	/** Initiate orthographic projection. automatically set it to screen size. */
	[[noreturn]] void InitiateOrthographicProjection();

	/** Initiate perspective projection matrix. automatically set it to screen size ratio. */
	[[noreturn]] void InitiatePerspectiveProjection();

    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(component::Component, Camera)
};

}

#endif /** OPGL16_SYSTEM_COMPONENTS_CAMERA_H */

