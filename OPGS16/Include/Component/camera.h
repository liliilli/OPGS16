#ifndef OPGL16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H
#define OPGL16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System\Components\camera_object.h
 * @brief Camera component file.
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and implement basic features.
 * 2018-03-11 Corection of ::opgs16::element::CObject class.
 * 2018-04-16 Add capability of controlling rear, far distance, and fov when Camera is Perspective.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <glm\glm.hpp>

#include <Component\Internal\component.h>   /// ::opgs16::component::CComponent
#include <Component\Internal\component_macro.h> /// Component macroes
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
namespace component {

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @class CameraObject
 * @brief Refined camera object class, different legacy camera object is independent of m_object_list.
 * You can set it up between two types, perspective (view-frustum) and orthographic (view-cube).
 *
 * If you see 3D world more realistic, set PERSPECTIVE. Otherwise if you set UI components with
 * no distortion, 3D world or 2D world with no sight distortion, set ORTHO.
 *
 * @log
 * 2018-02-14 Implemented basic features.
 * 2018-02-14 Move class from ::component to ::opgs16::component.
 * 2018-04-16 Add capability of controlling rear, far distance, and fov when Camera is Perspective.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
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

	CCamera(element::CObject& bound_obj, ViewType view_type, CameraType camera_type, bool _auto = true);
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

    glm::vec3 m_world{};
    glm::vec3 m_look{0, 0, -1};
    mutable glm::vec3 m_world_look{};

	float m_near, m_far;			/*! Distance sets region of sight. used only for perspective */
    float m_fov;                    /*! Angle sets how to see world, used only for perspective */
    bool  m_view_changed{ false };
    bool  m_proj_changed{ false };

    static bool s_main_camera_initiated;/** Check flag if main camera is already initiated */

    /*! Inherited via component::CComponent */
    void Update(float delta_time) override;

private:
    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CCamera)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif /** OPGL16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H */

