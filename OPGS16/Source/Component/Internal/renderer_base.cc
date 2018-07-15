
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/renderer_base.cc
///
/// @author Jongmin Yun
/// @log
/// 2018-03-15 Create file
/// 2018-04-06 Suppress warning, size_t to unsigned int warning.
/// 2018-04-29 Implement SetRenderFrameBuffer(const char*), Add GetRenderFrameBufferName()
///

/// Header file
#include <Component/Internal/renderer_base.h>

/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::manager::MSettingManager
#include <Manager/setting_manager.h>
/// ::opgs16::manager::prerendering namespace
#include <Manager/prerendering_manager.h>

namespace opgs16::component::_internal {

CRendererBase::~CRendererBase() = default;

void CRendererBase::SetRenderingLayer(const std::string& layer_name) {
  const auto& layer_list = manager::setting::GetRenderingLayerNameList();

  int32_t i = 0;
  const int32_t list_size = static_cast<int32_t>(layer_list.size());
  for (; i < list_size; ++i) {
    if (layer_name == layer_list[i]) {
      m_render_layer_index = i;
      break;
    }
  }

  if (i >= list_size) {
    PUSH_LOG_ERROR_EXT("Could not find rendering layer, {0}.", layer_name);
    m_render_layer_index = 0;
  }
}

void CRendererBase::SetRenderingLayer(int32_t layer_index) {
  PHITOS_ASSERT(layer_index >= 0, "layer index value must be 0 or positive.");

  const auto list_size = manager::setting::GetRenderingLayerNameListSize();
  m_render_layer_index = layer_index >= list_size ? 0 : layer_index;
}

const std::string& CRendererBase::GetRenderingLayerNameOf() const noexcept {
  return manager::setting::GetRenderingLayerName(m_render_layer_index);
}

void CRendererBase::SetRenderFrameBuffer(const char* frame_buffer_name) {
    /*! If frame_buffer_name is empty, set to default. */
    if (!std::strlen(frame_buffer_name)) {
        m_render_frame_buffer_name = "";
    }

    /*! Find whethere or not find frame buffer has frame_buffer_name already registered. */
    if (manager::prerendering::IsFrameBufferExist(frame_buffer_name)) {
        m_render_frame_buffer_name = frame_buffer_name;
    }
    else {
        //auto& resource_manager = manager::MResourceManager::Instance();
        //if (resource_manager.IsPreProcessingContainer(frame_buffer_name)) {
        //    auto* ptr = manager::prerendering::GenerateFrameBuffer(
        //        frame_buffer_name,
        //        resource_manager.GetPreProcessingContainer(frame_buffer_name));
        //    /*! Check FrameBuffer is properly created. */
        //    if (ptr)
        //        m_render_frame_buffer_name = frame_buffer_name;
        //}
        {
#if defined(_DEBUG)
            std::string error_message{ "Failed to invoke pre-processing buffer : " };
            error_message += frame_buffer_name;
            PUSH_LOG_ERRO(error_message.c_str());
            PUSH_LOG_ERRO("Automatically bind to default frame buffer.");
#endif
            m_render_frame_buffer_name = "";
        }
    }
}

} /// ::opgs16::component::_internal namespace