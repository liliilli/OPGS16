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

//!
//! Forward declaration
//!

namespace opgs16::component::_internal {
class CColliderBase;
}

//!
//! Implementation
//!

namespace opgs16::component::_internal {

class CPrivateAabbRenderer2D final : public CPrivateAabbRendererBase {
public:
  CPrivateAabbRenderer2D(element::CObject& bind_object,
                         CColliderBase* parent);
  ~CPrivateAabbRenderer2D();

  CPrivateAabbRenderer2D(const CPrivateAabbRenderer2D&) = default;
  CPrivateAabbRenderer2D(CPrivateAabbRenderer2D&&)      = default;
  CPrivateAabbRenderer2D& operator=(const CPrivateAabbRenderer2D&) = default;
  CPrivateAabbRenderer2D& operator=(CPrivateAabbRenderer2D&&)      = default;

private:
  void Render() override final;

  /// Shader is in ShaderManager, render sprite.
  element::CShaderWrapper m_wrapper;
  /// Quad VAO to render sprite on screen.
  element::CVaoContainer* m_weak_vao_ref = nullptr;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CPrivateAabbRendererBase, CPrivateAabbRenderer2D)
};

} /// ::opgs16::component::_internal namespace

#endif /// OPGS16_COMPONENT_INTERNAL_AABB_RENDERER_2D_H
