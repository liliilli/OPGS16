#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/camera.h
///
/// @brief Camera component file.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-14 Create file and implement basic features.
/// 2018-03-11 Corection of ::opgs16::element::CObject class.
/// 2018-04-16 Add capability of controlling rear, far distance, and fov when Camera is Perspective.
/// 2018-07-01 Refactoring glm::vec3 to DVector3.
///

#include <glm/glm.hpp>

/// ::opgs16::component::CComponent
#include <Component/Internal/component.h>
/// Component macroes
#include <Component/Internal/component_macro.h>
/// Forward declaration
#include <opgs16fwd.h>

/// ::opgs16::DVector3
#include <Helper/Type/vector3.h>

namespace opgs16::component {

///
/// @class CCamera
/// @brief Refined camera object class, different legacy camera object is independent of m_object_list.
/// You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
///
/// If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
/// no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
///
/// @log
/// 2018-02-14 Implemented basic features.
/// 2018-02-14 Move class from ::component to ::opgs16::component.
/// 2018-04-16 Add capability of controlling rear, far distance, and fov when Camera is Perspective.
/// 2018-07-01 Refactoring glm::vec3 to DVector3.
///
class CCamera final : public _internal::CComponent {
public:
	/*! Camera type how to see worlds. */
	enum class ViewType {
		PERSPECTIVE, /** This camera uses perspective camera, using view-frustum. */
		ORTHO		 /** This camera uses orthographic camera, using view-cube. */
	};

	/*! Camera priority type. */
	enum class CameraType {
		MAIN,	/** Main camera, this must just be only one in one scene. (even hierarchy) */
		SUB,	/** Sub camera, this type permits one more cameras in one scene. */
	};

	CCamera(element::CObject& bound_obj,
          ViewType view_type, CameraType camera_type);

	virtual ~CCamera();

    /*! Get View matrix */
    const glm::mat4& ViewMatrix() const noexcept {
        return m_view;
    }

    /*! Get Projection matrix */
	const glm::mat4& ProjectionMatrix() const noexcept {
        return m_projection;
    }

    /*! Get PV matrix */
    const glm::mat4& PvMatrix() const noexcept {
        return m_pv;
    }

    void SetWorldPosition(const glm::vec3& position) noexcept {
        m_world = position;
        m_world_look = m_look + m_world;
        m_view_changed = true;
    }

    void SetLookDirection(const glm::vec3& look) noexcept {
        m_look = look;
        m_world_look = m_look + m_world;
        m_view_changed = true;
    }

    /*!
     * @brief Set rear distance value.
     * If CCamera type is not PERSPECTIVE, ignore it and output warning log in debug mode.
     */
    void SetRear(const float value);

    /*!
     * @brief Set rear distance value.
     * If CCamera type is not PERSPECTIVE, ignore it and output warning log in debug mode.
     */
    void SetFar(const float value);

    /*!
     * @brief Set field of view value.
     * If CCamera type is not PERSPECTIVE, ignore it and output warning log in debug mode.
     * Also, this value is range from 0.f ~ 90.f; otherwise it will output warning log and clamped.
     */
    void SetFov(const float value);

private:
	mutable ViewType m_viewtype;	/** View type variable */
	mutable CameraType m_cameratype;/** Camera type variable */

    glm::mat4 m_view{};             /*! View matrix */
	glm::mat4 m_projection{};		/** Projection matrix world space or ui canvas to screen. */
    glm::mat4 m_pv{};               /*! Projection * View matrix */

  DVector3 m_world = {};
  DVector3 m_look = DVector3{0.f, 0.f, -1.f};
  mutable DVector3 m_world_look = {};

  /*! Distance sets region of sight. used only for perspective */
	float m_near = 0.1f;
  float m_far = 100.f;

  /*! Angle sets how to see world, used only for perspective */
  float m_fov = 70.f;
  bool  m_view_changed{ false };
  bool  m_proj_changed{ false };

    static bool s_main_camera_initiated;/** Check flag if main camera is already initiated */

    /*! Inherited via component::CComponent */
    void Update(float delta_time) override;

private:
    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CCamera)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H

