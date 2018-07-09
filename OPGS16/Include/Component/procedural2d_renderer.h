#ifndef OPGS16_COMPONENT_PROCEDURAL2D_RENDERER_H
#define OPGS16_COMPONENT_PROCEDURAL2D_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/procedural2d_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-09 Create file.
///

#include <Component/Internal/renderer_base.h>
#include <Component/Internal/component_macro.h>

#include <Helper/Type/vectori2.h>
#include <Helper/Type/color32.h>

namespace opgs16::component {

struct DTexelInfo final {
  DVectorInt2 position;
  DColor32 color32;
};

///
/// @class CProcedural2DRenderer
///
/// @brief
/// Render texture which has been created by run-time and procedurally.
///
class CProcedural2DRenderer final : public _internal::CRendererBase {
public:
  CProcedural2DRenderer(element::CObject& bind_object,
                        const std::string& shader_name,
                        const int32_t render_layer = 0);
  ~CProcedural2DRenderer();

  ///
  /// @brief Set texture size to be rendered.
  ///
  void SetSize(int32_t width, int32_t height);

  ///
  /// @brief
  ///
  void UpdateBuffer(const std::vector<DTexelInfo>& texels);

  ///
  /// @brief
  ///
  void UpdateBuffer(const DTexelInfo& texel);

  ///
  /// @brief
  ///
  void SetShader(const std::string& shader_name);

  ///
  /// @brief
  ///
  void RenderSprite();

  ///
  /// @brief
  ///
  element::CShaderWrapper* GetWrapper() {
    return &m_wrapper;
  }

private:
  std::vector<std::array<int8_t, 4>> m_buffer;

  element::CShaderWrapper m_wrapper;
  element::CVaoContainer* m_weak_vao_ref = nullptr;

  uint32_t m_texture_id = 0;
  GLenum  m_primitive_mode = GL_TRIANGLES;

  DVectorInt2 m_buffer_size = {};
  bool m_is_setup = false;
  bool m_is_dirty = false;

  void CreateTexture();
  void UpdateTexture();

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CProcedural2DRenderer)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PROCEDURAL2D_RENDERER_H