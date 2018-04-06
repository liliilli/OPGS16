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

/*!
 * @file System/Components/Internal/renderer_base.cc
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-15 Create file
 * 2018-04-06 Suppress warning, size_t to unsigned int warning.
 */

#include "renderer_base.h"                          /*! Header file */
#include "../../Element/Public/object.h"            /*! ::opgs16::element::CObject */
#include "../../Manager/Public/setting_manager.h"   /*! ::opgs16::manager::MSettingManager */

namespace opgs16 {
namespace component {
namespace _internal {

CRendererBase::CRendererBase(element::CObject& bind_object, const unsigned render_layer) :
    CComponent{ bind_object }, m_render_layer_index{ render_layer } {};

CRendererBase::~CRendererBase() {};

void CRendererBase::SetRenderLayer(const std::string& layer_name) {
    auto& layer_list = manager::MSettingManager::Instance().RenderingLayerNameList();
    unsigned i = 0;
    for (; i < layer_list.size(); ++i) {
        if (layer_name == layer_list[i]) {
            m_render_layer_index = i;
            break;
        }
    }

    if (i == layer_list.size()) m_render_layer_index = 0;
}

void CRendererBase::SetRenderLayer(const unsigned layer_index) {
    const auto list_size = manager::MSettingManager::Instance().RenderingLayerNameList().size();
    m_render_layer_index = (layer_index >= list_size) ? 0 : layer_index;
}

std::string CRendererBase::RenderLayerNameOf() const {
    return manager::MSettingManager::Instance().RenderingLayerName(m_render_layer_index);
}

} /*! opgs16::component::_internal */
} /*! opgs16::component */
} /*! opgs16 */
