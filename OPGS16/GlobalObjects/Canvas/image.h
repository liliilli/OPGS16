#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H

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
 * @file GlobalObjects\Canvas\image.h
 * @brief Image object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Remove Draw(ShaderNew) obsolete method.
 *            Replace Draw(), Update() with Render() and LocalUpdate()
 * 2018-03-11 Correction of UiObject namespace hierarchy positioning, and path.
 */

#include "../../System/Components/Public/sprite_renderer.h" /*! Sprite2DRenderer */
#include "../../System/Element/Public/ui_object.h"          /*! ::opgs16::element::UiObject */
#include "../../Headers/Fwd/objectfwd.h"                    /*! Canvas::Canvas */

namespace canvas {

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
class Image : public opgs16::element::UiObject {
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
	opgs16::component::Sprite2DRenderer m_sprite_renderer;	/** Sprite Renderer */
	Canvas* const m_ref_canvas;		/** l_value reference of canvas to get projection matrix. */

};
}

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_IMAGE_H */
