
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/sprite_renderer.cc
///
/// @brief Definition file of sprite2d_renderer.h
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
#include <Component/sprite2d_renderer.h>

/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::manager::TextureManager
#include <Manager/texture_manager.h>
/// ::opgs16::manager::ShaderManager
#include <Manager/shader_manager.h>
/// logger
#include <Headers/import_logger.h>

/// ::opgs16::element::builtin::BModel2DQuad
#include <Element/Default/model_2dquad.h>
/// ::opgs16::manager::_internal::vao namespace
#include <Manager/Internal/vao_management.h>

namespace opgs16::component {

CSprite2DRenderer::CSprite2DRenderer(
    element::CObject& bind_object,
    const std::string& sprite_tag,
    const std::string& shader_tag,
    const int32_t texture_index, const int32_t layer) :
    CRendererBase{ bind_object } {
  using manager::_internal::vao::FindVaoResource;
  using manager::shader::GetShader;
  using builtin::model::BModel2DQuad;
  using phitos::enums::EFound;

  auto [ptr, result] = FindVaoResource(BModel2DQuad::m_model_name.data());
  PHITOS_ASSERT(result == EFound::Found,
                "Did not find built-in vao items. opQuad2D.");
  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  SetTexture(sprite_tag);
  SetShader(shader_tag);
  m_wrapper.SetAttribute(m_weak_vao_ref);

  SetTextureFragmentIndex(texture_index);
}

element::CShaderWrapper& CSprite2DRenderer::GetWrapper() noexcept {
  return m_wrapper;
}

void CSprite2DRenderer::SetTexture(const std::string& texture_name) {
  const auto texture = manager::TextureManager::Instance().GetTexture(texture_name);
  if (!texture) {
    m_sprite = manager::TextureManager::Instance().GetTexture("opSystem");
  }
  m_sprite = texture;
  SetTextureFragmentIndex(0);
}

const std::string& CSprite2DRenderer::GetTextureName() const noexcept {
  if (!m_sprite) {
    PHITOS_UNEXPECTED_BRANCH();
  }

  return "";
}

int32_t CSprite2DRenderer::GetTextureFragmentIndex() const noexcept {
  return m_texture_fragment_index;
}

void CSprite2DRenderer::SetTextureFragmentIndex(int32_t index_value) {
  PHITOS_ASSERT(index_value >= 0,
                "Texture fragment index must be bigger than 0 or equal.");

  if (!m_sprite->DoesHasAtlas()) {
    PUSH_LOG_WARN(
        "Bound texture does not have atlas information.\n"
        "so failed to assign new_index.");
    m_texture_fragment_index = 0;
    m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
    m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
    return;
  }

  m_texture_fragment_index = index_value;

  using ETexelType = texture::CTexture2D::ETexelType;
  const auto texel_ptr_ld = m_sprite->GetTexelPtr(ETexelType::LEFT_DOWN, index_value);
  const auto texel_ptr_ru = m_sprite->GetTexelPtr(ETexelType::RIGHT_UP, index_value);

  if (texel_ptr_ld && texel_ptr_ru) {
    m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ texel_ptr_ld[0], texel_ptr_ld[1] });
    m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ texel_ptr_ru[0], texel_ptr_ru[1] });
  }
  else {
    PUSH_LOG_WARN(
        "Any getting texel from resource has been failed.\n"
        "Texel is assigned to overall region.");
    m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
    m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
  };
}

void CSprite2DRenderer::RenderSprite() {
  if (m_weak_vao_ref == nullptr)
    return;

  // The name is incorrect
  m_wrapper.UseShader();

  for (const auto& vao : m_weak_vao_ref->GetVaoList()) {
    glBindVertexArray(vao.GetVaoId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sprite->Id());

    glDrawElementsInstanced(
        m_primitive_enum,
        6,
        GL_UNSIGNED_INT,
        nullptr,
        m_instance_count);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void CSprite2DRenderer::SetShader(const std::string& shader_name) {
  m_wrapper.SetShader(manager::shader::GetShader(shader_name));
}

void CSprite2DRenderer::SetPrimitiveMode(EPrimitiveType primitive_type) {
  m_primitive_type = primitive_type;

  switch (m_primitive_type) {
  case EPrimitiveType::Point:
    m_primitive_enum = GL_POINT;
    break;
  case EPrimitiveType::Line:
    m_primitive_enum = GL_LINE;
    break;
  case EPrimitiveType::LineLoop:
    m_primitive_enum = GL_LINE_LOOP;
    break;
  case EPrimitiveType::Triangle:
    m_primitive_enum = GL_TRIANGLES;
    break;
  }
}

EPrimitiveType CSprite2DRenderer::GetPrimitiveMode() const noexcept {
  return m_primitive_type;
}

void CSprite2DRenderer::SetInstanceCount(int32_t instance_count) {
  PHITOS_ASSERT(instance_count >= 1, "Instance count must be bigger than 1 or equal.");
  m_instance_count = instance_count;
}

int32_t CSprite2DRenderer::GetInstanceCount() const noexcept {
  return m_instance_count;
}

CSprite2DRenderer::~CSprite2DRenderer() {
  if (m_weak_vao_ref != nullptr) {
    m_weak_vao_ref->DecreaseCount();
  }
}

} /// ::opgs16::component namespace

