#ifndef OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H
#define OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Components/Internal/renderer_base.h
 *
 * @log
 * 2018-03-15 Create file.
 * 2018-04-06 Suppress warning, size_t to unsigned int warning.
 * 2018-04-08 Add comment
 * 2018-04-29 Add SetRenderFrameBuffer(const char*), Add GetRenderFrameBufferName();
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Manager\object_manager.h>       /// ::opgs16::manager::MObjectManager
#include <Manager\prerendering_manager.h> /// ::opgs16::manager::MPrerenderingManager

/// opgs16::component::_internal::CComponent
#include <Component\Internal\component.h>
#include <Component\Internal\component_macro.h>

namespace opgs16 {
namespace component {
namespace _internal {

/*!
* @class CRendererBase
* @brief Base abstract class which is being base class of CRenderer derived classes.
* @author Jongmin Yun
*/
class CRendererBase : public CComponent {
public:
    CRendererBase(element::CObject& bind_object, const unsigned render_layer);
    virtual ~CRendererBase() = 0;

    /*!
     * @brief Set rendering layer with name.
     * if render layer is out of bound or not found, set value to 0
     * @param[in] layer_name Name.
     */
    void SetRenderLayer(const std::string& layer_name);

    /*!
     * @brief Set render layer with value. if render layer is out of bound, set value to 0.
     * @param[in] layer_index Value.
     */
    void SetRenderLayer(const unsigned layer_index);

    /*! Get Rendering layer index value of this instance. */
    size_t RenderLayerIndexOf() const noexcept {
        return m_render_layer_index;
    }

    /*! Get Rendering layer name value of this instance. */
    std::string RenderLayerNameOf() const;

    /*!
     * @brief Set frame buffer to be rendered in. 
     * If frame_buffer_name is empty, the object which binds this component will be rendered onto default frame buffer;
     * Otherwise find whether or not pre-processing frame buffer is exist and target to specific pre-proc buffer.
     * If not find any matched pre-proc buffer, set target to default frame buffer with warning message.
     *
     * @param[in] frame_buffer_name Frame buffer name to target. If empty, set target to default.
     */
    void SetRenderFrameBuffer(const char* frame_buffer_name = "");

    /*! Get frame buffer name to which the object is being bound. */
    const std::string& GetRenderFrameBufferName() const noexcept {
        return m_render_frame_buffer_name;
    }

    void Update() override final {
        // If render frame buffer name is empty, regard it as default buffer and bind.
        if (m_render_frame_buffer_name.empty()) {
            using manager::MObjectManager;
            MObjectManager::Instance().InsertRenderingObject(&GetObject(), m_render_layer_index);
        }
        else {
            auto buffer = manager::prerendering::GetFrameBuffer(m_render_frame_buffer_name.c_str());
            buffer->RegisterDisposableObject(GetObject());
        }
    };

private:
    /*! Rendering layer index */
    unsigned    m_render_layer_index{ 0 }; 
    /*! Frame buffer name onto which to be rendered */
    std::string m_render_frame_buffer_name{};

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CRendererBase)
};

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */


#endif // !OPGS16_SYSTEM_COMPONENTS_INTERNAL_RENDERER_BASE_H