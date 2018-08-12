#ifndef OPGS16_COMPONENT_INTERNAL_XYZ_AXIS_RENDERER_H
#define OPGS16_COMPONENT_INTERNAL_XYZ_AXIS_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/xyz_axis_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file.
///

#include <Component/Internal/renderer_base.h>
#include <Helper/Type/vector3.h>

namespace opgs16::component::_internal {

class CPrivateXyzAxisRenderer final : public CRendererBase {
SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CPrivateXyzAxisRenderer)
public:
  CPrivateXyzAxisRenderer(element::CObject& bind_object);

  ///
  /// @brief
  ///
  void Render();

private:
  ///
  /// @brief
  ///
  void Update(float delta_time) override final;

  DVector3 m_x_normalized;
  DVector3 m_y_normalized;
  DVector3 m_z_normalized;

  element::CShaderWrapper m_wrapper;
  element::CVaoContainer* m_weak_vao_ref = nullptr;
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_INTERNAL_XYZ_AXIS_RENDERER_H