#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H

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
 * @file System/Components/Public/empty_renderer.h
 * @brief Empty renderer does nothing but just inherits CRendererBase.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-15 Create file, Make class.
 */

/// ::opgs16::component::_internal::CComponent
#include <Component\Internal\renderer_base.h>
#include <Component\Internal\component_macro.h> /// Component macroes

namespace opgs16 {
namespace component {

/*!
 * @class CEmptyRenderer
 * @log
 * 2018-03-15 Make class.
 */
class CEmptyRenderer final : public _internal::CRendererBase {
public:
	/*! Make EmptyRenderer instance. (Constructor) */
	CEmptyRenderer(element::CObject& bind_object, const unsigned layer = 0);

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CEmptyRenderer)
};

} /*! opgs16::component */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H

