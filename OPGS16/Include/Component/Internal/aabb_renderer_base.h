#ifndef OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H
#define OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/aabb_renderer_base.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file.
///

#include <Component/Internal/renderer_base.h>

namespace opgs16::component::_internal {

class CPrivateAabbRendererBase : public opgs16::component::_internal::CRendererBase {
public:
  ~CPrivateAabbRendererBase() = 0;

private:

};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_BASE_H