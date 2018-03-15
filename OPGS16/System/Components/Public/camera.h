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

/*!
 * @file System\Components\camera_object.h
 * @brief Camera component file.
 * @author Jongmin Yun
 * @log
 * 2018-02-14 Create file and implement basic features.
 * 2018-03-11 Corection of ::opgs16::element::CObject class.
 */

#include <glm\glm.hpp>
#include "../Internal/component.h"          /*! component::Component */
#include "../Internal/component_macro.h"    /*! SET_UP_HASH_MEMBER_DERIVED() */
#include "../../Headers/Fwd/objectfwd.h"    /*! Object as reference */

namespace opgs16 {
namespace component {

/**
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
 */
class CCamera final : public _internal::CComponent {
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

	CCamera(element::CObject& bound_obj, ViewType view_type,
            CameraType camera_type, bool _auto = true);
	virtual ~CCamera();
    virtual void Update() override;     /*! Inherited via component::CComponent */

    const glm::mat4& ViewMatrix() const noexcept;         /*! Get View matrix */
	const glm::mat4& ProjectionMatrix() const noexcept;   /*! Get Projection matrix */
    const glm::mat4& PvMatrix() const noexcept;                 /*! Get PV matrix */

    inline void SetWorldPosition(const glm::vec3& position) {
        m_world = position;
        m_world_look = m_look + m_world;
        m_information_changed = true;
    }

    inline void SetLookDirection(const glm::vec3& look) {
        m_look = look;
        m_world_look = m_look + m_world;
        m_information_changed = true;
    }

private:
	mutable ViewType m_viewtype;	/** View type variable */
	mutable CameraType m_cameratype;/** Camera type variable */

    glm::mat4 m_view{};             /*! View matrix */
	glm::mat4 m_projection{};		/** Projection matrix world space or ui canvas to screen. */
    glm::mat4 m_PV{};               /*! Projection * View matrix */

    glm::vec3 m_world{};
    glm::vec3 m_look{};
    mutable glm::vec3 m_world_look{};

	float m_near, m_far;			/*! Distance sets region of sight. used only for perspective */
	float m_perspective_angle;		/*! Angle sets how to see world, used only for perspective */
    bool  m_information_changed{ false };

    static bool s_main_camera_initiated;/** Check flag if main camera is already initiated */

private:
    /*! Create members related to type hash value. */
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CCamera)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif /** OPGL16_SYSTEM_COMPONENTS_PUBLIC_CAMERA_H */

