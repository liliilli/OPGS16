
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Element/Private/prerendering_container.cc
* @brief Container for storing CPreRenderingFrame classes
*
* @log
* 2018-04-29 Create file. Implement basic mechanisms of CPreRenderingContainer.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\prerendering_container.h> /// Header file

#include <utility>

/// ::opgs16::element::CObject
#include <Element\object.h>

namespace opgs16::element {

void CPreRenderingContainer::Initialize() {
    // Make final output texture.
    std::unique_ptr<CFrameBuferFrame> final_frame_buffer = std::make_unique<CFrameBuferFrame>();
    final_frame_buffer->GenerateFrameBuffer(0);
    final_frame_buffer->GenerateDefaultColorBuffer();
    final_frame_buffer->BindTextureToFrameBuffer(0, 0, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);

    final_frame_buffer->InitializeDefaultDepthBuffer();
    final_frame_buffer->SetShader("opQuad2d");
    final_frame_buffer->Initialize();
    m_frame_buffer.emplace_back(std::move(final_frame_buffer));

    // Final
    m_initialized = true;
}

void CPreRenderingContainer::Update() {
    for (auto& object : m_frame_buffer) {
        object->Update();
    }
}

void CPreRenderingContainer::Render() {
    if (m_frame_buffer.empty()) return;

    // First, bind start framebuffer and render bound objects.
    (*m_frame_buffer.begin())->Bind();
    for (auto& object : m_binding_objects) object->Draw();

    auto it_effect = m_frame_buffer.begin();
    auto it_rbegin = (--m_frame_buffer.end());

    std::for_each(m_frame_buffer.begin(), m_frame_buffer.end(), [&](decltype(*it_effect)& effect) {
        if (it_effect == it_rbegin) return;

        (*std::next(it_effect))->Bind();
        effect->RenderEffect();
        ++it_effect;
    });

    // Final
    CleanBoundObject();
}

unsigned CPreRenderingContainer::GetOutputTextureId() const noexcept {
    return (*m_frame_buffer.rbegin())->GetTexture(0)->Id();
}

void CPreRenderingContainer::CleanBoundObject() {
    m_binding_objects.clear();
}

} /*! opgs16::element */