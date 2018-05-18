
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Manager/Private/prerendering_manager.cc
 * @brief Definition file of ../Public/prerendering_manager.h
 * @log
 * 2018-04-21 Create file.
 * 2018-04-29 Create framebuffer_list and Implement IsFrameBufferExist(const char*).
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Manager\prerendering_manager.h>   /// Header file

#include <map>

namespace {
/*!  */
std::map<std::string, opgs16::element::CPreRenderingContainer> m_preprocessing_framebuffer_list{};

} /*! unnamed namespace */

namespace opgs16::manager::prerendering {

bool IsFrameBufferExist(const char* frame_buffer_name) noexcept {
    return m_preprocessing_framebuffer_list.find(frame_buffer_name) != m_preprocessing_framebuffer_list.end();
}

element::CPreRenderingContainer* GenerateFrameBuffer(const char* frame_buffer_name) {
    element::CPreRenderingContainer* buffer_ptr{ nullptr };

    if (!IsFrameBufferExist(frame_buffer_name)) {
        m_preprocessing_framebuffer_list[frame_buffer_name] = {};
        buffer_ptr = &m_preprocessing_framebuffer_list[frame_buffer_name];
    }

    return buffer_ptr;
}

element::CPreRenderingContainer* GetFrameBuffer(const char* frame_buffer_name) noexcept {
    element::CPreRenderingContainer* buffer_ptr{ nullptr };
    if (IsFrameBufferExist(frame_buffer_name))
        buffer_ptr = &m_preprocessing_framebuffer_list[frame_buffer_name];

    return buffer_ptr;
}

void Update() {
    for (auto&[key, container] : m_preprocessing_framebuffer_list) {
        if (container.IsWorkable())
            container.Update();
    }
}

void Render() {
    for (auto& [key, container] : m_preprocessing_framebuffer_list) {
        if (container.IsWorkable())
            container.Render();
    }
}

} /*! opgs16::manager::prerendering */