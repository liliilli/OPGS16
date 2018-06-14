
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/sprite_renderer.cc
///
/// @brief Definition file of sprite_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-07 Move file to /Component and Add boilerplate comments.
/// 2018-04-06
/// Abandon IndexSize structure indicates texture index,
/// and replace it with one unsigned value.
/// 2018-06-13
/// Integrate mesh, model manager and object and weak vao.
///

/// Header file
#include <Component/sprite_renderer.h>

/// ::opgs16::component::_internal::CSpriteRendererImpl
#include <Component/Impl/sprite_renderer_impl.h>
/// ::opgs16::element::CObject
#include <Element/object.h>

namespace opgs16::component {

CSprite2DRenderer::CSprite2DRenderer(
    element::CObject& bind_object,
    const std::string& sprite_tag,
    const std::string& shader_tag,
    const unsigned texture_index,
    const unsigned render_layer) :
    CRendererBase{ bind_object, render_layer },
  m_impl { new _internal::CSpriteRendererImpl(sprite_tag, shader_tag, texture_index) } {
}

element::CShaderWrapper& CSprite2DRenderer::Wrapper() const {
  return m_impl->Wrapper();
}

void CSprite2DRenderer::SetTexture(const std::string& texture_name) {
  return m_impl->SetTexture(texture_name);
}

const unsigned CSprite2DRenderer::TextureIndex() const noexcept {
  return m_impl->TextureIndex();
}

void CSprite2DRenderer::SetTextureIndex(const unsigned index_value) {
  m_impl->SetTextureIndex(index_value);
}

void CSprite2DRenderer::RenderSprite() {
  m_impl->RenderSprite();
}

void CSprite2DRenderer::SetShader(const std::string& shader_name) {
  m_impl->SetShader(shader_name);
}

void CSprite2DRenderer::SetInstanceCount(unsigned instance_count) {
  m_impl->SetInstanceCount(instance_count);
}

CSprite2DRenderer::~CSprite2DRenderer() = default;

} /// ::opgs16::component namespace

