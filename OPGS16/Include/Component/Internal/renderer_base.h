#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/renderer_base.h
///
/// @log
/// 2018-03-15 Create file.
/// 2018-04-06 Suppress warning, size_t to unsigned int warning.
/// 2018-04-08 Add comment
/// 2018-04-29 Add SetRenderFrameBuffer(const char*), Add GetRenderFrameBufferName();
///
/// @todo Implement SetRenderFrameBuffer function.
///

/// opgs16::component::_internal::CComponent
#include <Component/Internal/component.h>
#include <Component/Internal/component_macro.h>

/// ::opgs16::manager::MObjectManager
#include <Manager/object_manager.h>
/// ::opgs16::manager::MPrerenderingManager
#include <Manager/prerendering_manager.h>

namespace opgs16::component::_internal {

///
/// @class CRendererBase
/// @brief Base abstract class which is being base class of CRenderer derived classes.
/// @author Jongmin Yun
///
class CRendererBase : public CComponent {
public:
  CRendererBase(element::CObject& bind_object) : CComponent{ bind_object } {};

  virtual ~CRendererBase() = 0;

  ///
  /// @brief Set rendering layer with name.
  /// if render layer is out of bound or not found, set value to 0
  /// @param[in] layer_name Name.
  ///
  void SetRenderingLayer(const std::string& layer_name);

  ///
  /// @brief Set render layer with value. if render layer is out of bound, set value to 0.
  /// @param[in] layer_index Value.
  ///
  void SetRenderingLayer(int32_t layer_index);

  int32_t GetRenderingLayerIndexOf() const noexcept {
    return m_render_layer_index;
  }

  const std::string& GetRenderingLayerNameOf() const noexcept;

  ///
  /// @brief Set frame buffer to be rendered in.
  /// If frame_buffer_name is empty, the object which binds this component will be rendered onto default frame buffer;
  /// Otherwise find whether or not pre-processing frame buffer is exist and target to specific pre-proc buffer.
  /// If not find any matched pre-proc buffer, set target to default frame buffer with warning message.
  ///
  /// @param[in] frame_buffer_name Frame buffer name to target. If empty, set target to default.
  ///
  void SetRenderFrameBuffer(const char* frame_buffer_name = "");

  const std::string& GetRenderFrameBufferName() const noexcept {
    return m_render_frame_buffer_name;
  }

  void Update(float delta_time) override {
    // If render frame buffer name is empty, regard it as default buffer and bind.
    if (m_render_frame_buffer_name.empty()) {
      manager::object::InsertRenderingObject(&GetBindObject(), m_render_layer_index);
    }
    else {
      auto buffer = manager::prerendering::GetFrameBuffer(m_render_frame_buffer_name.c_str());
      buffer->RegisterDisposableObject(GetBindObject());
    }
  };

private:
  int32_t m_render_layer_index = 0;
  std::string m_render_frame_buffer_name{};

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRendererBase)
};

} /// ::opgs16::component::_internal namespace


#endif /// !OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H