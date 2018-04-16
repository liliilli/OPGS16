#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H

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
 * @file GlobalObjects\Canvas\image.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
 * 2018-03-11 Refactoring.
 * 2018-04-16 Move ::canvas::CCanvas to ::opgs16::element::canvas::CCanvas.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../Headers/Fwd/objectfwd.h"    /*! glm::mat4
                                              * helper::CShaderNew;
                                              * camera::CameraObject; */
#include "../../System/Element/Public/ui_object.h"  /*! ::opgs16::element::UiObject */

namespace opgs16 {
namespace element {
namespace canvas {
/**
 * @class CCanvas
 * @brief Canvas frame object saves and manages UI components and permit accessing from UI m_object_list.
 *
 * This canvas offers several features. (and comming soon features)
 * 1. Each update frame, canvas updates orthographic projection size
 * 2. Each draw frame, canvas object calls UI components.
 * 3. UI components can access Canvas object (parent), get orthographic projection.
 * 4. when Canvas object is destroyed, children UI components are automatically disorganized.
 *
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method. Replace with Render(), LocalUpdate()
 * 2018-04-16 Move ::canvas::CCanvas to ::opgs16::element::canvas::CCanvas.
 */
class CCanvas : public opgs16::element::UiObject {
public:
	/**
	 * @brief Canvas constructor.
	 * CCanvas has orthographic camera fixed to screen size, this camera will be initialized.
	 */
	CCanvas();
	virtual ~CCanvas() = default;

	virtual void LocalUpdate() override;

    /*! Disable DEPTH_TEST until All UiObject rendering is done. */
	virtual void Render() override;

	/** Get Ui camera's orthographic projection matrix. */
	const glm::mat4 GetUiCameraPVMatrix() const;

private:
	/** Camera object uses just one orthographic type. */
    opgs16::component::CCamera* m_camera{ nullptr };

	bool m_is_size_changed;
};

} /*! opgs16::element::canvas */
} /*! opgs16::element */
} /*! opgs16 */

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_CANVAS_H */