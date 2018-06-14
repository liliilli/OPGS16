
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Impl/sprite_renderer_impl.cc
///
/// @author Jongmin Yun
/// @log
/// 2018-03-10 Refactoring. Move implementations to ::opgs16::component::_internal
/// 2018-04-02 std::string to std::wstring for UTF-16 characters.
/// 2018-04-06 Support to CTexture2DAtlas information. Revise SetTextureIndex
/// 2018-04-08 Supporting change of shader on running.
///

/// Header file
#include <Component/Impl/sprite_renderer_impl.h>

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

namespace opgs16::component::_internal {

CSpriteRendererImpl::CSpriteRendererImpl(const std::string& sprite_tag,
                                         const std::string& shader_tag,
                                         uint32_t texture_index) :
    m_sprite{ manager::TextureManager::Instance().GetTexture(sprite_tag) },
    m_texture_fragment_index{ texture_index } {
  using manager::_internal::vao::FindVaoResource;
  using builtin::model::BModel2DQuad;

  auto [ptr, result] = FindVaoResource(BModel2DQuad::m_model_name.data());
  if (result == phitos::enums::EFound::NotFound) {
    PHITOS_ASSERT(result == phitos::enums::EFound::Found,
        "Did not find built-in vao items. opQuad2D.");
  }

  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(manager::shader::GetShader(shader_tag));
  m_wrapper.SetAttribute(m_weak_vao_ref);

  SetTextureIndex(texture_index);
}

CSpriteRendererImpl::~CSpriteRendererImpl() {
  if (m_weak_vao_ref != nullptr) {
    m_weak_vao_ref->DecreaseCount();
  }
}

void CSpriteRendererImpl::SetTextureIndex(const uint32_t new_index) noexcept {
  if (!m_sprite->DoesHasAtlas()) {
    PUSH_LOG_WARN(
        "Bound texture does not have atlas information.\n"
        "so failed to assign new_index.");
    m_texture_fragment_index = 0;
    m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
    m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
  }
  else {
    m_texture_fragment_index = new_index;

    using ETexelType = texture::CTexture2D::ETexelType;
    const auto texel_ptr_ld = m_sprite->GetTexelPtr(ETexelType::LEFT_DOWN, new_index);
    const auto texel_ptr_ru = m_sprite->GetTexelPtr(ETexelType::RIGHT_UP, new_index);
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
    }
  }
}

void CSpriteRendererImpl::SetTexture(const std::string& texture_name) noexcept {
  m_sprite = manager::TextureManager::Instance().GetTexture(texture_name);
  SetTextureIndex(0);
}

void CSpriteRendererImpl::SetShader(const std::string& shader_name) {
  m_wrapper.SetShader(manager::shader::GetShader(shader_name));
}

void CSpriteRendererImpl::RenderSprite() {
  if (m_weak_vao_ref == nullptr) return;

  // The name is incorrect
  m_wrapper.UseShader();

  for (const auto& vao : m_weak_vao_ref->GetVaoList()) {
    glBindVertexArray(vao.GetVaoId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sprite->Id());

    glDrawElements(m_primitive_mode, 6, GL_UNSIGNED_INT, nullptr);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component::_internal namespace