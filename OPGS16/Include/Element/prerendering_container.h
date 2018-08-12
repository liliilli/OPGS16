#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_PRERENDERING_CONTAINER_H
#define OPGS16_SYSTEM_ELEMENT_PUBLIC_PRERENDERING_CONTAINER_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Element/prerendering_container.h
 * @brief Container for storing CPreRenderingFrame classes
 *
 * @log
 * 2018-04-29 Create file. Create CPreRenderingContainer class.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <vector>

/// ::opgs16::element::CFrameBufferFrame
#include <Shader\framebuffer.h>
#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16::element {

/*!
 * @class CPreRenderingContainer 
 */
class CPreRenderingContainer final {
public:
    void Initialize();

    bool IsWorkable() const noexcept {
        return m_initialized;
    }

    bool IsBoundObjectExist() const noexcept {
        return m_binding_objects.empty();
    }

    void RegisterDisposableObject(CObject& object) {
        m_binding_objects.emplace_back(&object);
    }

    template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<element::CFrameBufferFrame, _Ty>>>
    void InsertFrameBuferFrame() {
        m_frame_buffer.push_back(std::make_unique<_Ty>());
        (*m_frame_buffer.rbegin())->Initialize();
    }

    unsigned GetOutputTextureId() const noexcept;

    void CleanBoundObject();

    /*! Update container. */
    void Update();

    /*! Render container. */
    void Render();

private:
    bool m_initialized{ false };

    std::vector<CObject*> m_binding_objects{};
    std::vector<std::unique_ptr<element::CFrameBufferFrame>> m_frame_buffer;
};

} /*! ::opgs16::element */

#endif // OPGS16_SYSTEM_ELEMENT_PUBLIC_PRERENDERING_CONTAINER_H