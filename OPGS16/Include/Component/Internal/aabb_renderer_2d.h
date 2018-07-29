#ifndef OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_2D_H
#define OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_2D_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/aabb_renderer_2d.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file
///

#include <Component/Internal/aabb_renderer_base.h>

namespace opgs16::component::_internal {

class CPrivateAabbRenderer2D final :
      public opgs16::component::_internal::CPrivateAabbRendererBase {
public:
  CPrivateAabbRenderer2D();

private:

};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_2D_H
