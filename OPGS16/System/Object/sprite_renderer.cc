#include "sprite_renderer.h"                /*! Header file */
#include "Impl\sprite_renderer_impl.h"      /*! SpriteRendererImpl */

void SpriteRendererImplDeleter::operator()(SpriteRendererImpl* p) { delete p; }

SpriteRenderer::SpriteRenderer(const std::string& sprite_tag,
                               const std::string& shader_tag,
                               const resource::Texture2D::IndexSize& texture_index,
                               const unsigned layer) :
    m_impl { new SpriteRendererImpl(sprite_tag, shader_tag, texture_index, layer) } {
}

void SpriteRenderer::SetLayer(const unsigned layer) {
    m_impl->SetLayer(layer);
}

const unsigned SpriteRenderer::GetLayer() const {
    return m_impl->GetLayer();
}

ShaderWrapper& SpriteRenderer::GetWrapper() const {
    return m_impl->GetWrapper();
}

const SpriteRenderer::IndexSize& SpriteRenderer::GetTextureIndex() const noexcept {
    return m_impl->GetTextureIndex();
}

void SpriteRenderer::SetTextureIndex(const IndexSize& new_index) {
    m_impl->SetTextureIndex(new_index);
}

void SpriteRenderer::RenderSprite() {
    m_impl->RenderSprite();
}
