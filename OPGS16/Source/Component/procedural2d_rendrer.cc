#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/procedural2d_rendrer.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-09 Create file.
///

/// Header file
#include <Component/procedural2d_renderer.h>

#include <Phitos/Dbg/assert.h>

#include <Element/Default/model_2dquad.h>
#include <Manager/shader_manager.h>
#include <Manager/Internal/vao_management.h>

namespace opgs16::component {

CProcedural2DRenderer::CProcedural2DRenderer(element::CObject& bind_object,
                                             const std::string& shader_name) :
    CRendererBase{ bind_object } {
  using manager::_internal::vao::FindVaoResource;
  using builtin::model::BModel2DQuad;

  auto [ptr, result] = FindVaoResource(BModel2DQuad::m_model_name.data());
  if (result == phitos::enums::EFound::NotFound) {
    PHITOS_ASSERT(result == phitos::enums::EFound::Found,
                  "Did not find built-in vao items.");
  }
  m_weak_vao_ref = ptr;
  m_weak_vao_ref->IncreaseCount();
  m_weak_vao_ref->SetDirty();

  m_wrapper.SetShader(manager::shader::GetShader(shader_name));
  m_wrapper.SetAttribute(m_weak_vao_ref);
  m_wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
  m_wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
}

CProcedural2DRenderer::~CProcedural2DRenderer() {
  if (m_weak_vao_ref != nullptr) {
    m_weak_vao_ref->DecreaseCount();
  }
}

void CProcedural2DRenderer::SetSize(int32_t width, int32_t height) {
  PHITOS_ASSERT(width > 0 && height > 0, "Width and Height must be bigger than 0.");

  if (!m_is_setup) {
    m_is_setup = true;
  }

  if (!m_is_dirty) {
    m_is_dirty = true;
  }

  m_buffer.clear();
  m_buffer.resize(width * height);
  m_buffer_size.x = width;
  m_buffer_size.y = height;

  for (auto& element : m_buffer) {
    element[3] = 0b11111111;
  }

  if (m_texture_id > 0) {
    glDeleteTextures(1, &m_texture_id);
    m_texture_id = 0;
  }
}

void CProcedural2DRenderer::UpdateBuffer(const std::vector<DTexelInfo>& texels) {
  for (const auto& texel : texels) {
    UpdateBuffer(texel);
  }
}

void CProcedural2DRenderer::UpdateBuffer(const DTexelInfo& texel) {
  const auto y = texel.position.y % m_buffer_size.y;
  const auto x = texel.position.x % m_buffer_size.x;
  const auto index = x + y * m_buffer_size.x;

  m_buffer[index][0] = texel.color32.R();
  m_buffer[index][1] = texel.color32.G();
  m_buffer[index][2] = texel.color32.B();
  m_buffer[index][3] = texel.color32.A();

  m_is_dirty = true;
}

void CProcedural2DRenderer::SetShader(const std::string& shader_name) {
  m_wrapper.SetShader(manager::shader::GetShader(shader_name));
}

void CProcedural2DRenderer::RenderSprite() {
  if (m_weak_vao_ref == nullptr) return;
  if (m_is_dirty) {
    if (m_texture_id == 0) {
      glGenTextures(1, &m_texture_id);
      glBindTexture(GL_TEXTURE_2D, m_texture_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                   this->m_buffer_size.x, this->m_buffer_size.y,
                   0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer.data());
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (m_texture_id > 0) {
      glBindTexture(GL_TEXTURE_2D, m_texture_id);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                      this->m_buffer_size.x, this->m_buffer_size.y,
                      GL_RGBA, GL_UNSIGNED_BYTE, m_buffer.data());
    }

    m_is_dirty = false;
  }

  m_wrapper.UseShader();

  for (const auto& vao : m_weak_vao_ref->GetVaoList()) {
    glBindVertexArray(vao.GetVaoId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glDrawElements(m_primitive_mode, 6, GL_UNSIGNED_INT, nullptr);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

} /// ::opgs16::component namespace