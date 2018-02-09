#include "sprite_renderer.h"                /*! Header file */
#include "Impl\sprite_renderer_impl.h"      /*! SpriteRendererImpl */

void SpriteRendererImplDeleter::operator()(SpriteRendererImpl* p) { delete p; }

SpriteRenderer::SpriteRenderer(const std::string& sprite_tag,
							   const std::string& shader_tag,
							   const unsigned layer) {
    /*! Body */
    pimpl_type instance{ new SpriteRendererImpl(sprite_tag, shader_tag, layer) };
    m_impl = std::move(instance);
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

void SpriteRenderer::RenderSprite() {
    m_impl->RenderSprite();
}
