#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H


///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/empty_renderer.h
///
/// @brief IsSceneEmpty renderer does nothing but just inherits CRendererBase.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-15 Create file, Make class.
///

/// ::opgs16::component::_internal::CComponent
#include <Component/Internal/renderer_base.h>
#include <Component/Internal/component_macro.h>

namespace opgs16::component {

///
///@class CEmptyRenderer
///
class CEmptyRenderer final : public _internal::CRendererBase {
public:
	CEmptyRenderer(element::CObject& bind_object);

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CEmptyRenderer)
};

} /// ::opgs16::component namespace

#endif /// !OPGS16_SYSTEM_COMPONENTS_PUBLIC_EMPTY_RENDERER_H

